http_res_get_intercept_rule(struct proxy *px, struct list *rules, struct session *s, struct http_txn *txn)
{
	struct connection *cli_conn;
	struct http_res_rule *rule;
	struct hdr_ctx ctx;

	list_for_each_entry(rule, rules, list) {
		if (rule->action >= HTTP_RES_ACT_MAX)
			continue;

		/* check optional condition */
		if (rule->cond) {
			int ret;

			ret = acl_exec_cond(rule->cond, px, s, txn, SMP_OPT_DIR_RES|SMP_OPT_FINAL);
			ret = acl_pass(ret);

			if (rule->cond->pol == ACL_COND_UNLESS)
				ret = !ret;

			if (!ret) /* condition not matched */
				continue;
		}


		switch (rule->action) {
		case HTTP_RES_ACT_ALLOW:
			return NULL; /* "allow" rules are OK */

		case HTTP_RES_ACT_DENY:
			txn->flags |= TX_SVDENY;
			return rule;

		case HTTP_RES_ACT_SET_NICE:
			s->task->nice = rule->arg.nice;
			break;

		case HTTP_RES_ACT_SET_TOS:
			if ((cli_conn = objt_conn(s->req->prod->end)) && conn_ctrl_ready(cli_conn))
				inet_set_tos(cli_conn->t.sock.fd, cli_conn->addr.from, rule->arg.tos);
			break;

		case HTTP_RES_ACT_SET_MARK:
#ifdef SO_MARK
			if ((cli_conn = objt_conn(s->req->prod->end)) && conn_ctrl_ready(cli_conn))
				setsockopt(cli_conn->t.sock.fd, SOL_SOCKET, SO_MARK, &rule->arg.mark, sizeof(rule->arg.mark));
#endif
			break;

		case HTTP_RES_ACT_SET_LOGL:
			s->logs.level = rule->arg.loglevel;
			break;

		case HTTP_RES_ACT_REPLACE_HDR:
		case HTTP_RES_ACT_REPLACE_VAL:
			if (http_transform_header(s, &txn->rsp, rule->arg.hdr_add.name, rule->arg.hdr_add.name_len,
			                          txn->rsp.chn->buf->p, &txn->hdr_idx, &rule->arg.hdr_add.fmt,
			                          &rule->arg.hdr_add.re, &ctx, rule->action))
				return NULL; /* note: we should report an error here */
			break;

		case HTTP_RES_ACT_DEL_HDR:
		case HTTP_RES_ACT_SET_HDR:
			ctx.idx = 0;
			/* remove all occurrences of the header */
			while (http_find_header2(rule->arg.hdr_add.name, rule->arg.hdr_add.name_len,
						 txn->rsp.chn->buf->p, &txn->hdr_idx, &ctx)) {
				http_remove_header2(&txn->rsp, &txn->hdr_idx, &ctx);
			}
			if (rule->action == HTTP_RES_ACT_DEL_HDR)
				break;
			/* now fall through to header addition */

		case HTTP_RES_ACT_ADD_HDR:
			chunk_printf(&trash, "%s: ", rule->arg.hdr_add.name);
			memcpy(trash.str, rule->arg.hdr_add.name, rule->arg.hdr_add.name_len);
			trash.len = rule->arg.hdr_add.name_len;
			trash.str[trash.len++] = ':';
			trash.str[trash.len++] = ' ';
			trash.len += build_logline(s, trash.str + trash.len, trash.size - trash.len, &rule->arg.hdr_add.fmt);
			http_header_add_tail2(&txn->rsp, &txn->hdr_idx, trash.str, trash.len);
			break;

		case HTTP_RES_ACT_DEL_ACL:
		case HTTP_RES_ACT_DEL_MAP: {
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

		case HTTP_RES_ACT_ADD_ACL: {
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
			/* check if the entry already exists */
			if (pat_ref_find_elt(ref, key) == NULL)
				pat_ref_add(ref, key, NULL, NULL);

			break;
			}

		case HTTP_RES_ACT_SET_MAP: {
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

		case HTTP_RES_ACT_CUSTOM_CONT:
			rule->action_ptr(rule, px, s, txn);
			break;

		case HTTP_RES_ACT_CUSTOM_STOP:
			rule->action_ptr(rule, px, s, txn);
			return rule;
		}
	}

	/* we reached the end of the rules, nothing to report */
	return NULL;
}
