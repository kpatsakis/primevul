http_req_get_intercept_rule(struct proxy *px, struct list *rules, struct session *s, struct http_txn *txn)
{
	struct connection *cli_conn;
	struct http_req_rule *rule;
	struct hdr_ctx ctx;
	const char *auth_realm;

	list_for_each_entry(rule, rules, list) {
		if (rule->action >= HTTP_REQ_ACT_MAX)
			continue;

		/* check optional condition */
		if (rule->cond) {
			int ret;

			ret = acl_exec_cond(rule->cond, px, s, txn, SMP_OPT_DIR_REQ|SMP_OPT_FINAL);
			ret = acl_pass(ret);

			if (rule->cond->pol == ACL_COND_UNLESS)
				ret = !ret;

			if (!ret) /* condition not matched */
				continue;
		}


		switch (rule->action) {
		case HTTP_REQ_ACT_ALLOW:
			return HTTP_RULE_RES_STOP;

		case HTTP_REQ_ACT_DENY:
			return HTTP_RULE_RES_DENY;

		case HTTP_REQ_ACT_TARPIT:
			txn->flags |= TX_CLTARPIT;
			return HTTP_RULE_RES_DENY;

		case HTTP_REQ_ACT_AUTH:
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
			stream_int_retnclose(&s->si[0], &trash);
			session_inc_http_err_ctr(s);
			return HTTP_RULE_RES_ABRT;

		case HTTP_REQ_ACT_REDIR:
			if (!http_apply_redirect_rule(rule->arg.redir, s, txn))
				return HTTP_RULE_RES_BADREQ;
			return HTTP_RULE_RES_DONE;

		case HTTP_REQ_ACT_SET_NICE:
			s->task->nice = rule->arg.nice;
			break;

		case HTTP_REQ_ACT_SET_TOS:
			if ((cli_conn = objt_conn(s->req->prod->end)) && conn_ctrl_ready(cli_conn))
				inet_set_tos(cli_conn->t.sock.fd, cli_conn->addr.from, rule->arg.tos);
			break;

		case HTTP_REQ_ACT_SET_MARK:
#ifdef SO_MARK
			if ((cli_conn = objt_conn(s->req->prod->end)) && conn_ctrl_ready(cli_conn))
				setsockopt(cli_conn->t.sock.fd, SOL_SOCKET, SO_MARK, &rule->arg.mark, sizeof(rule->arg.mark));
#endif
			break;

		case HTTP_REQ_ACT_SET_LOGL:
			s->logs.level = rule->arg.loglevel;
			break;

		case HTTP_REQ_ACT_REPLACE_HDR:
		case HTTP_REQ_ACT_REPLACE_VAL:
			if (http_transform_header(s, &txn->req, rule->arg.hdr_add.name, rule->arg.hdr_add.name_len,
			                          txn->req.chn->buf->p, &txn->hdr_idx, &rule->arg.hdr_add.fmt,
			                          &rule->arg.hdr_add.re, &ctx, rule->action))
				return HTTP_RULE_RES_BADREQ;
			break;

		case HTTP_REQ_ACT_DEL_HDR:
		case HTTP_REQ_ACT_SET_HDR:
			ctx.idx = 0;
			/* remove all occurrences of the header */
			while (http_find_header2(rule->arg.hdr_add.name, rule->arg.hdr_add.name_len,
						 txn->req.chn->buf->p, &txn->hdr_idx, &ctx)) {
				http_remove_header2(&txn->req, &txn->hdr_idx, &ctx);
			}
			if (rule->action == HTTP_REQ_ACT_DEL_HDR)
				break;
			/* now fall through to header addition */

		case HTTP_REQ_ACT_ADD_HDR:
			chunk_printf(&trash, "%s: ", rule->arg.hdr_add.name);
			memcpy(trash.str, rule->arg.hdr_add.name, rule->arg.hdr_add.name_len);
			trash.len = rule->arg.hdr_add.name_len;
			trash.str[trash.len++] = ':';
			trash.str[trash.len++] = ' ';
			trash.len += build_logline(s, trash.str + trash.len, trash.size - trash.len, &rule->arg.hdr_add.fmt);
			http_header_add_tail2(&txn->req, &txn->hdr_idx, trash.str, trash.len);
			break;

		case HTTP_REQ_ACT_DEL_ACL:
		case HTTP_REQ_ACT_DEL_MAP: {
			struct pat_ref *ref;
			char *key;
			int len;

			/* collect reference */
			ref = pat_ref_lookup(rule->arg.map.ref);
			if (!ref)
				continue;

			/* collect key */
			len = build_logline(s, trash.str, trash.size, &rule->arg.map.key);
			key = trash.str;
			key[len] = '\0';

			/* perform update */
			/* returned code: 1=ok, 0=ko */
			pat_ref_delete(ref, key);

			break;
			}

		case HTTP_REQ_ACT_ADD_ACL: {
			struct pat_ref *ref;
			char *key;
			struct chunk *trash_key;
			int len;

			trash_key = get_trash_chunk();

			/* collect reference */
			ref = pat_ref_lookup(rule->arg.map.ref);
			if (!ref)
				continue;

			/* collect key */
			len = build_logline(s, trash_key->str, trash_key->size, &rule->arg.map.key);
			key = trash_key->str;
			key[len] = '\0';

			/* perform update */
			/* add entry only if it does not already exist */
			if (pat_ref_find_elt(ref, key) == NULL)
				pat_ref_add(ref, key, NULL, NULL);

			break;
			}

		case HTTP_REQ_ACT_SET_MAP: {
			struct pat_ref *ref;
			char *key, *value;
			struct chunk *trash_key, *trash_value;
			int len;

			trash_key = get_trash_chunk();
			trash_value = get_trash_chunk();

			/* collect reference */
			ref = pat_ref_lookup(rule->arg.map.ref);
			if (!ref)
				continue;

			/* collect key */
			len = build_logline(s, trash_key->str, trash_key->size, &rule->arg.map.key);
			key = trash_key->str;
			key[len] = '\0';

			/* collect value */
			len = build_logline(s, trash_value->str, trash_value->size, &rule->arg.map.value);
			value = trash_value->str;
			value[len] = '\0';

			/* perform update */
			if (pat_ref_find_elt(ref, key) != NULL)
				/* update entry if it exists */
				pat_ref_set(ref, key, value, NULL);
			else
				/* insert a new entry */
				pat_ref_add(ref, key, value, NULL);

			break;
			}

		case HTTP_REQ_ACT_CUSTOM_CONT:
			rule->action_ptr(rule, px, s, txn);
			break;

		case HTTP_REQ_ACT_CUSTOM_STOP:
			rule->action_ptr(rule, px, s, txn);
			return HTTP_RULE_RES_DONE;
		}
	}

	/* we reached the end of the rules, nothing to report */
	return HTTP_RULE_RES_CONT;
}
