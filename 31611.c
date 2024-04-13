http_req_get_intercept_rule(struct proxy *px, struct list *rules, struct stream *s, int *deny_status)
{
	struct session *sess = strm_sess(s);
	struct http_txn *txn = s->txn;
	struct connection *cli_conn;
	struct act_rule *rule;
	struct hdr_ctx ctx;
	const char *auth_realm;
	int act_flags = 0;
	int len;

	/* If "the current_rule_list" match the executed rule list, we are in
	 * resume condition. If a resume is needed it is always in the action
	 * and never in the ACL or converters. In this case, we initialise the
	 * current rule, and go to the action execution point.
	 */
	if (s->current_rule) {
		rule = s->current_rule;
		s->current_rule = NULL;
		if (s->current_rule_list == rules)
			goto resume_execution;
	}
	s->current_rule_list = rules;

	list_for_each_entry(rule, rules, list) {

		/* check optional condition */
		if (rule->cond) {
			int ret;

			ret = acl_exec_cond(rule->cond, px, sess, s, SMP_OPT_DIR_REQ|SMP_OPT_FINAL);
			ret = acl_pass(ret);

			if (rule->cond->pol == ACL_COND_UNLESS)
				ret = !ret;

			if (!ret) /* condition not matched */
				continue;
		}

		act_flags |= ACT_FLAG_FIRST;
resume_execution:
		switch (rule->action) {
		case ACT_ACTION_ALLOW:
			return HTTP_RULE_RES_STOP;

		case ACT_ACTION_DENY:
			if (deny_status)
				*deny_status = rule->deny_status;
			return HTTP_RULE_RES_DENY;

		case ACT_HTTP_REQ_TARPIT:
			txn->flags |= TX_CLTARPIT;
			if (deny_status)
				*deny_status = rule->deny_status;
			return HTTP_RULE_RES_DENY;

		case ACT_HTTP_REQ_AUTH:
			/* Auth might be performed on regular http-req rules as well as on stats */
			auth_realm = rule->arg.auth.realm;
			if (!auth_realm) {
				if (px->uri_auth && rules == &px->uri_auth->http_req_rules)
					auth_realm = STATS_DEFAULT_REALM;
				else
					auth_realm = px->id;
			}
			/* send 401/407 depending on whether we use a proxy or not. We still
			 * count one error, because normal browsing won't significantly
			 * increase the counter but brute force attempts will.
			 */
			chunk_printf(&trash, (txn->flags & TX_USE_PX_CONN) ? HTTP_407_fmt : HTTP_401_fmt, auth_realm);
			txn->status = (txn->flags & TX_USE_PX_CONN) ? 407 : 401;
			http_reply_and_close(s, txn->status, &trash);
			stream_inc_http_err_ctr(s);
			return HTTP_RULE_RES_ABRT;

		case ACT_HTTP_REDIR:
			if (!http_apply_redirect_rule(rule->arg.redir, s, txn))
				return HTTP_RULE_RES_BADREQ;
			return HTTP_RULE_RES_DONE;

		case ACT_HTTP_SET_NICE:
			s->task->nice = rule->arg.nice;
			break;

		case ACT_HTTP_SET_TOS:
			if ((cli_conn = objt_conn(sess->origin)) && conn_ctrl_ready(cli_conn))
				inet_set_tos(cli_conn->handle.fd, &cli_conn->addr.from, rule->arg.tos);
			break;

		case ACT_HTTP_SET_MARK:
#ifdef SO_MARK
			if ((cli_conn = objt_conn(sess->origin)) && conn_ctrl_ready(cli_conn))
				setsockopt(cli_conn->handle.fd, SOL_SOCKET, SO_MARK, &rule->arg.mark, sizeof(rule->arg.mark));
#endif
			break;

		case ACT_HTTP_SET_LOGL:
			s->logs.level = rule->arg.loglevel;
			break;

		case ACT_HTTP_REPLACE_HDR:
		case ACT_HTTP_REPLACE_VAL:
			if (http_transform_header(s, &txn->req, rule->arg.hdr_add.name,
			                          rule->arg.hdr_add.name_len,
			                          &rule->arg.hdr_add.fmt,
			                          &rule->arg.hdr_add.re, rule->action))
				return HTTP_RULE_RES_BADREQ;
			break;

		case ACT_HTTP_DEL_HDR:
			ctx.idx = 0;
			/* remove all occurrences of the header */
			while (http_find_header2(rule->arg.hdr_add.name, rule->arg.hdr_add.name_len,
						 txn->req.chn->buf->p, &txn->hdr_idx, &ctx)) {
				http_remove_header2(&txn->req, &txn->hdr_idx, &ctx);
			}
			break;

		case ACT_HTTP_SET_HDR:
		case ACT_HTTP_ADD_HDR: {
			/* The scope of the trash buffer must be limited to this function. The
			 * build_logline() function can execute a lot of other function which
			 * can use the trash buffer. So for limiting the scope of this global
			 * buffer, we build first the header value using build_logline, and
			 * after we store the header name.
			 */
			struct chunk *replace;

			replace = alloc_trash_chunk();
			if (!replace)
				return HTTP_RULE_RES_BADREQ;

			len = rule->arg.hdr_add.name_len + 2,
			len += build_logline(s, replace->str + len, replace->size - len, &rule->arg.hdr_add.fmt);
			memcpy(replace->str, rule->arg.hdr_add.name, rule->arg.hdr_add.name_len);
			replace->str[rule->arg.hdr_add.name_len] = ':';
			replace->str[rule->arg.hdr_add.name_len + 1] = ' ';
			replace->len = len;

			if (rule->action == ACT_HTTP_SET_HDR) {
				/* remove all occurrences of the header */
				ctx.idx = 0;
				while (http_find_header2(rule->arg.hdr_add.name, rule->arg.hdr_add.name_len,
							 txn->req.chn->buf->p, &txn->hdr_idx, &ctx)) {
					http_remove_header2(&txn->req, &txn->hdr_idx, &ctx);
				}
			}

			http_header_add_tail2(&txn->req, &txn->hdr_idx, replace->str, replace->len);

			free_trash_chunk(replace);
			break;
			}

		case ACT_HTTP_DEL_ACL:
		case ACT_HTTP_DEL_MAP: {
			struct pat_ref *ref;
			struct chunk *key;

			/* collect reference */
			ref = pat_ref_lookup(rule->arg.map.ref);
			if (!ref)
				continue;

			/* allocate key */
			key = alloc_trash_chunk();
			if (!key)
				return HTTP_RULE_RES_BADREQ;

			/* collect key */
			key->len = build_logline(s, key->str, key->size, &rule->arg.map.key);
			key->str[key->len] = '\0';

			/* perform update */
			/* returned code: 1=ok, 0=ko */
			HA_SPIN_LOCK(PATREF_LOCK, &ref->lock);
			pat_ref_delete(ref, key->str);
			HA_SPIN_UNLOCK(PATREF_LOCK, &ref->lock);

			free_trash_chunk(key);
			break;
			}

		case ACT_HTTP_ADD_ACL: {
			struct pat_ref *ref;
			struct chunk *key;

			/* collect reference */
			ref = pat_ref_lookup(rule->arg.map.ref);
			if (!ref)
				continue;

			/* allocate key */
			key = alloc_trash_chunk();
			if (!key)
				return HTTP_RULE_RES_BADREQ;

			/* collect key */
			key->len = build_logline(s, key->str, key->size, &rule->arg.map.key);
			key->str[key->len] = '\0';

			/* perform update */
			/* add entry only if it does not already exist */
			HA_SPIN_LOCK(PATREF_LOCK, &ref->lock);
			if (pat_ref_find_elt(ref, key->str) == NULL)
				pat_ref_add(ref, key->str, NULL, NULL);
			HA_SPIN_UNLOCK(PATREF_LOCK, &ref->lock);

			free_trash_chunk(key);
			break;
			}

		case ACT_HTTP_SET_MAP: {
			struct pat_ref *ref;
			struct chunk *key, *value;

			/* collect reference */
			ref = pat_ref_lookup(rule->arg.map.ref);
			if (!ref)
				continue;

			/* allocate key */
			key = alloc_trash_chunk();
			if (!key)
				return HTTP_RULE_RES_BADREQ;

			/* allocate value */
			value = alloc_trash_chunk();
			if (!value) {
				free_trash_chunk(key);
				return HTTP_RULE_RES_BADREQ;
			}

			/* collect key */
			key->len = build_logline(s, key->str, key->size, &rule->arg.map.key);
			key->str[key->len] = '\0';

			/* collect value */
			value->len = build_logline(s, value->str, value->size, &rule->arg.map.value);
			value->str[value->len] = '\0';

			/* perform update */
			if (pat_ref_find_elt(ref, key->str) != NULL)
				/* update entry if it exists */
				pat_ref_set(ref, key->str, value->str, NULL);
			else
				/* insert a new entry */
				pat_ref_add(ref, key->str, value->str, NULL);

			free_trash_chunk(key);
			free_trash_chunk(value);
			break;
			}

		case ACT_CUSTOM:
			if ((s->req.flags & CF_READ_ERROR) ||
			    ((s->req.flags & (CF_SHUTR|CF_READ_NULL)) &&
			     !(s->si[0].flags & SI_FL_CLEAN_ABRT) &&
			     (px->options & PR_O_ABRT_CLOSE)))
				act_flags |= ACT_FLAG_FINAL;

			switch (rule->action_ptr(rule, px, s->sess, s, act_flags)) {
			case ACT_RET_ERR:
			case ACT_RET_CONT:
				break;
			case ACT_RET_STOP:
				return HTTP_RULE_RES_DONE;
			case ACT_RET_YIELD:
				s->current_rule = rule;
				return HTTP_RULE_RES_YIELD;
			}
			break;

		case ACT_ACTION_TRK_SC0 ... ACT_ACTION_TRK_SCMAX:
			/* Note: only the first valid tracking parameter of each
			 * applies.
			 */

			if (stkctr_entry(&s->stkctr[trk_idx(rule->action)]) == NULL) {
				struct stktable *t;
				struct stksess *ts;
				struct stktable_key *key;
				void *ptr1, *ptr2;

				t = rule->arg.trk_ctr.table.t;
				key = stktable_fetch_key(t, s->be, sess, s, SMP_OPT_DIR_REQ | SMP_OPT_FINAL, rule->arg.trk_ctr.expr, NULL);

				if (key && (ts = stktable_get_entry(t, key))) {
					stream_track_stkctr(&s->stkctr[trk_idx(rule->action)], t, ts);

					/* let's count a new HTTP request as it's the first time we do it */
					ptr1 = stktable_data_ptr(t, ts, STKTABLE_DT_HTTP_REQ_CNT);
					ptr2 = stktable_data_ptr(t, ts, STKTABLE_DT_HTTP_REQ_RATE);
					if (ptr1 || ptr2) {
						HA_RWLOCK_WRLOCK(STK_SESS_LOCK, &ts->lock);

						if (ptr1)
							stktable_data_cast(ptr1, http_req_cnt)++;

						if (ptr2)
							update_freq_ctr_period(&stktable_data_cast(ptr2, http_req_rate),
							                       t->data_arg[STKTABLE_DT_HTTP_REQ_RATE].u, 1);

						HA_RWLOCK_WRUNLOCK(STK_SESS_LOCK, &ts->lock);

						/* If data was modified, we need to touch to re-schedule sync */
						stktable_touch_local(t, ts, 0);
					}

					stkctr_set_flags(&s->stkctr[trk_idx(rule->action)], STKCTR_TRACK_CONTENT);
					if (sess->fe != s->be)
						stkctr_set_flags(&s->stkctr[trk_idx(rule->action)], STKCTR_TRACK_BACKEND);
				}
			}
			break;

		/* other flags exists, but normaly, they never be matched. */
		default:
			break;
		}
	}

	/* we reached the end of the rules, nothing to report */
	return HTTP_RULE_RES_CONT;
}
