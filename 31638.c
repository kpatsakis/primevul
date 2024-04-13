struct act_rule *parse_http_req_cond(const char **args, const char *file, int linenum, struct proxy *proxy)
{
	struct act_rule *rule;
	struct action_kw *custom = NULL;
	int cur_arg;
	char *error;

	rule = calloc(1, sizeof(*rule));
	if (!rule) {
		ha_alert("parsing [%s:%d]: out of memory.\n", file, linenum);
		goto out_err;
	}

	if (!strcmp(args[0], "allow")) {
		rule->action = ACT_ACTION_ALLOW;
		cur_arg = 1;
	} else if (!strcmp(args[0], "deny") || !strcmp(args[0], "block") || !strcmp(args[0], "tarpit")) {
		int code;
		int hc;

		if (!strcmp(args[0], "tarpit")) {
		    rule->action = ACT_HTTP_REQ_TARPIT;
		    rule->deny_status = HTTP_ERR_500;
		}
		else {
			rule->action = ACT_ACTION_DENY;
			rule->deny_status = HTTP_ERR_403;
		}
		cur_arg = 1;
                if (strcmp(args[cur_arg], "deny_status") == 0) {
                        cur_arg++;
                        if (!args[cur_arg]) {
                                ha_alert("parsing [%s:%d] : error detected in %s '%s' while parsing 'http-request %s' rule : missing status code.\n",
					 file, linenum, proxy_type_str(proxy), proxy->id, args[0]);
                                goto out_err;
                        }

                        code = atol(args[cur_arg]);
                        cur_arg++;
                        for (hc = 0; hc < HTTP_ERR_SIZE; hc++) {
                                if (http_err_codes[hc] == code) {
                                        rule->deny_status = hc;
                                        break;
                                }
                        }

                        if (hc >= HTTP_ERR_SIZE) {
                                ha_warning("parsing [%s:%d] : status code %d not handled, using default code %d.\n",
					   file, linenum, code, http_err_codes[rule->deny_status]);
                        }
                }
	} else if (!strcmp(args[0], "auth")) {
		rule->action = ACT_HTTP_REQ_AUTH;
		cur_arg = 1;

		while(*args[cur_arg]) {
			if (!strcmp(args[cur_arg], "realm")) {
				rule->arg.auth.realm = strdup(args[cur_arg + 1]);
				cur_arg+=2;
				continue;
			} else
				break;
		}
	} else if (!strcmp(args[0], "set-nice")) {
		rule->action = ACT_HTTP_SET_NICE;
		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg + 1] && strcmp(args[cur_arg + 1], "if") != 0 && strcmp(args[cur_arg + 1], "unless") != 0)) {
			ha_alert("parsing [%s:%d]: 'http-request %s' expects exactly 1 argument (integer value).\n",
				 file, linenum, args[0]);
			goto out_err;
		}
		rule->arg.nice = atoi(args[cur_arg]);
		if (rule->arg.nice < -1024)
			rule->arg.nice = -1024;
		else if (rule->arg.nice > 1024)
			rule->arg.nice = 1024;
		cur_arg++;
	} else if (!strcmp(args[0], "set-tos")) {
#ifdef IP_TOS
		char *err;
		rule->action = ACT_HTTP_SET_TOS;
		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg + 1] && strcmp(args[cur_arg + 1], "if") != 0 && strcmp(args[cur_arg + 1], "unless") != 0)) {
			ha_alert("parsing [%s:%d]: 'http-request %s' expects exactly 1 argument (integer/hex value).\n",
				 file, linenum, args[0]);
			goto out_err;
		}

		rule->arg.tos = strtol(args[cur_arg], &err, 0);
		if (err && *err != '\0') {
			ha_alert("parsing [%s:%d]: invalid character starting at '%s' in 'http-request %s' (integer/hex value expected).\n",
				 file, linenum, err, args[0]);
			goto out_err;
		}
		cur_arg++;
#else
		ha_alert("parsing [%s:%d]: 'http-request %s' is not supported on this platform (IP_TOS undefined).\n", file, linenum, args[0]);
		goto out_err;
#endif
	} else if (!strcmp(args[0], "set-mark")) {
#ifdef SO_MARK
		char *err;
		rule->action = ACT_HTTP_SET_MARK;
		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg + 1] && strcmp(args[cur_arg + 1], "if") != 0 && strcmp(args[cur_arg + 1], "unless") != 0)) {
			ha_alert("parsing [%s:%d]: 'http-request %s' expects exactly 1 argument (integer/hex value).\n",
				 file, linenum, args[0]);
			goto out_err;
		}

		rule->arg.mark = strtoul(args[cur_arg], &err, 0);
		if (err && *err != '\0') {
			ha_alert("parsing [%s:%d]: invalid character starting at '%s' in 'http-request %s' (integer/hex value expected).\n",
				 file, linenum, err, args[0]);
			goto out_err;
		}
		cur_arg++;
		global.last_checks |= LSTCHK_NETADM;
#else
		ha_alert("parsing [%s:%d]: 'http-request %s' is not supported on this platform (SO_MARK undefined).\n", file, linenum, args[0]);
		goto out_err;
#endif
	} else if (!strcmp(args[0], "set-log-level")) {
		rule->action = ACT_HTTP_SET_LOGL;
		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg + 1] && strcmp(args[cur_arg + 1], "if") != 0 && strcmp(args[cur_arg + 1], "unless") != 0)) {
		bad_log_level:
			ha_alert("parsing [%s:%d]: 'http-request %s' expects exactly 1 argument (log level name or 'silent').\n",
				 file, linenum, args[0]);
			goto out_err;
		}
		if (strcmp(args[cur_arg], "silent") == 0)
			rule->arg.loglevel = -1;
		else if ((rule->arg.loglevel = get_log_level(args[cur_arg]) + 1) == 0)
			goto bad_log_level;
		cur_arg++;
	} else if (strcmp(args[0], "add-header") == 0 || strcmp(args[0], "set-header") == 0) {
		rule->action = *args[0] == 'a' ? ACT_HTTP_ADD_HDR : ACT_HTTP_SET_HDR;
		cur_arg = 1;

		if (!*args[cur_arg] || !*args[cur_arg+1] ||
		    (*args[cur_arg+2] && strcmp(args[cur_arg+2], "if") != 0 && strcmp(args[cur_arg+2], "unless") != 0)) {
			ha_alert("parsing [%s:%d]: 'http-request %s' expects exactly 2 arguments.\n",
				 file, linenum, args[0]);
			goto out_err;
		}

		rule->arg.hdr_add.name = strdup(args[cur_arg]);
		rule->arg.hdr_add.name_len = strlen(rule->arg.hdr_add.name);
		LIST_INIT(&rule->arg.hdr_add.fmt);

		proxy->conf.args.ctx = ARGC_HRQ;
		error = NULL;
		if (!parse_logformat_string(args[cur_arg + 1], proxy, &rule->arg.hdr_add.fmt, LOG_OPT_HTTP,
		                            (proxy->cap & PR_CAP_FE) ? SMP_VAL_FE_HRQ_HDR : SMP_VAL_BE_HRQ_HDR, &error)) {
			ha_alert("parsing [%s:%d]: 'http-request %s': %s.\n",
				 file, linenum, args[0], error);
			free(error);
			goto out_err;
		}
		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;
		cur_arg += 2;
	} else if (strcmp(args[0], "replace-header") == 0 || strcmp(args[0], "replace-value") == 0) {
		rule->action = args[0][8] == 'h' ? ACT_HTTP_REPLACE_HDR : ACT_HTTP_REPLACE_VAL;
		cur_arg = 1;

		if (!*args[cur_arg] || !*args[cur_arg+1] || !*args[cur_arg+2] ||
		    (*args[cur_arg+3] && strcmp(args[cur_arg+3], "if") != 0 && strcmp(args[cur_arg+3], "unless") != 0)) {
			ha_alert("parsing [%s:%d]: 'http-request %s' expects exactly 3 arguments.\n",
				 file, linenum, args[0]);
			goto out_err;
		}

		rule->arg.hdr_add.name = strdup(args[cur_arg]);
		rule->arg.hdr_add.name_len = strlen(rule->arg.hdr_add.name);
		LIST_INIT(&rule->arg.hdr_add.fmt);

		error = NULL;
		if (!regex_comp(args[cur_arg + 1], &rule->arg.hdr_add.re, 1, 1, &error)) {
			ha_alert("parsing [%s:%d] : '%s' : %s.\n", file, linenum,
				 args[cur_arg + 1], error);
			free(error);
			goto out_err;
		}

		proxy->conf.args.ctx = ARGC_HRQ;
		error = NULL;
		if (!parse_logformat_string(args[cur_arg + 2], proxy, &rule->arg.hdr_add.fmt, LOG_OPT_HTTP,
		                            (proxy->cap & PR_CAP_FE) ? SMP_VAL_FE_HRQ_HDR : SMP_VAL_BE_HRQ_HDR, &error)) {
			ha_alert("parsing [%s:%d]: 'http-request %s': %s.\n",
				 file, linenum, args[0], error);
			free(error);
			goto out_err;
		}

		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;
		cur_arg += 3;
	} else if (strcmp(args[0], "del-header") == 0) {
		rule->action = ACT_HTTP_DEL_HDR;
		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg+1] && strcmp(args[cur_arg+1], "if") != 0 && strcmp(args[cur_arg+1], "unless") != 0)) {
			ha_alert("parsing [%s:%d]: 'http-request %s' expects exactly 1 argument.\n",
				 file, linenum, args[0]);
			goto out_err;
		}

		rule->arg.hdr_add.name = strdup(args[cur_arg]);
		rule->arg.hdr_add.name_len = strlen(rule->arg.hdr_add.name);

		proxy->conf.args.ctx = ARGC_HRQ;
		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;
		cur_arg += 1;
	} else if (strncmp(args[0], "track-sc", 8) == 0 &&
		 args[0][9] == '\0' && args[0][8] >= '0' &&
		 args[0][8] < '0' + MAX_SESS_STKCTR) { /* track-sc 0..9 */
		struct sample_expr *expr;
		unsigned int where;
		char *err = NULL;

		cur_arg = 1;
		proxy->conf.args.ctx = ARGC_TRK;

		expr = sample_parse_expr((char **)args, &cur_arg, file, linenum, &err, &proxy->conf.args);
		if (!expr) {
			ha_alert("parsing [%s:%d] : error detected in %s '%s' while parsing 'http-request %s' rule : %s.\n",
				 file, linenum, proxy_type_str(proxy), proxy->id, args[0], err);
			free(err);
			goto out_err;
		}

		where = 0;
		if (proxy->cap & PR_CAP_FE)
			where |= SMP_VAL_FE_HRQ_HDR;
		if (proxy->cap & PR_CAP_BE)
			where |= SMP_VAL_BE_HRQ_HDR;

		if (!(expr->fetch->val & where)) {
			ha_alert("parsing [%s:%d] : error detected in %s '%s' while parsing 'http-request %s' rule :"
				 " fetch method '%s' extracts information from '%s', none of which is available here.\n",
				 file, linenum, proxy_type_str(proxy), proxy->id, args[0],
				 args[cur_arg-1], sample_src_names(expr->fetch->use));
			free(expr);
			goto out_err;
		}

		if (strcmp(args[cur_arg], "table") == 0) {
			cur_arg++;
			if (!args[cur_arg]) {
				ha_alert("parsing [%s:%d] : error detected in %s '%s' while parsing 'http-request %s' rule : missing table name.\n",
					 file, linenum, proxy_type_str(proxy), proxy->id, args[0]);
				free(expr);
				goto out_err;
			}
			/* we copy the table name for now, it will be resolved later */
			rule->arg.trk_ctr.table.n = strdup(args[cur_arg]);
			cur_arg++;
		}
		rule->arg.trk_ctr.expr = expr;
		rule->action = ACT_ACTION_TRK_SC0 + args[0][8] - '0';
		rule->check_ptr = check_trk_action;
	} else if (strcmp(args[0], "redirect") == 0) {
		struct redirect_rule *redir;
		char *errmsg = NULL;

		if ((redir = http_parse_redirect_rule(file, linenum, proxy, (const char **)args + 1, &errmsg, 1, 0)) == NULL) {
			ha_alert("parsing [%s:%d] : error detected in %s '%s' while parsing 'http-request %s' rule : %s.\n",
				 file, linenum, proxy_type_str(proxy), proxy->id, args[0], errmsg);
			goto out_err;
		}

		/* this redirect rule might already contain a parsed condition which
		 * we'll pass to the http-request rule.
		 */
		rule->action = ACT_HTTP_REDIR;
		rule->arg.redir = redir;
		rule->cond = redir->cond;
		redir->cond = NULL;
		cur_arg = 2;
		return rule;
	} else if (strncmp(args[0], "add-acl", 7) == 0) {
		/* http-request add-acl(<reference (acl name)>) <key pattern> */
		rule->action = ACT_HTTP_ADD_ACL;
		/*
		 * '+ 8' for 'add-acl('
		 * '- 9' for 'add-acl(' + trailing ')'
		 */
		rule->arg.map.ref = my_strndup(args[0] + 8, strlen(args[0]) - 9);

		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg+1] && strcmp(args[cur_arg+1], "if") != 0 && strcmp(args[cur_arg+1], "unless") != 0)) {
			ha_alert("parsing [%s:%d]: 'http-request %s' expects exactly 1 argument.\n",
				 file, linenum, args[0]);
			goto out_err;
		}

		LIST_INIT(&rule->arg.map.key);
		proxy->conf.args.ctx = ARGC_HRQ;
		error = NULL;
		if (!parse_logformat_string(args[cur_arg], proxy, &rule->arg.map.key, LOG_OPT_HTTP,
		                            (proxy->cap & PR_CAP_FE) ? SMP_VAL_FE_HRQ_HDR : SMP_VAL_BE_HRQ_HDR, &error)) {
			ha_alert("parsing [%s:%d]: 'http-request %s': %s.\n",
				 file, linenum, args[0], error);
			free(error);
			goto out_err;
		}
		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;
		cur_arg += 1;
	} else if (strncmp(args[0], "del-acl", 7) == 0) {
		/* http-request del-acl(<reference (acl name)>) <key pattern> */
		rule->action = ACT_HTTP_DEL_ACL;
		/*
		 * '+ 8' for 'del-acl('
		 * '- 9' for 'del-acl(' + trailing ')'
		 */
		rule->arg.map.ref = my_strndup(args[0] + 8, strlen(args[0]) - 9);

		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg+1] && strcmp(args[cur_arg+1], "if") != 0 && strcmp(args[cur_arg+1], "unless") != 0)) {
			ha_alert("parsing [%s:%d]: 'http-request %s' expects exactly 1 argument.\n",
				 file, linenum, args[0]);
			goto out_err;
		}

		LIST_INIT(&rule->arg.map.key);
		proxy->conf.args.ctx = ARGC_HRQ;
		error = NULL;
		if (!parse_logformat_string(args[cur_arg], proxy, &rule->arg.map.key, LOG_OPT_HTTP,
		                            (proxy->cap & PR_CAP_FE) ? SMP_VAL_FE_HRQ_HDR : SMP_VAL_BE_HRQ_HDR, &error)) {
			ha_alert("parsing [%s:%d]: 'http-request %s': %s.\n",
				 file, linenum, args[0], error);
			free(error);
			goto out_err;
		}
		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;
		cur_arg += 1;
	} else if (strncmp(args[0], "del-map", 7) == 0) {
		/* http-request del-map(<reference (map name)>) <key pattern> */
		rule->action = ACT_HTTP_DEL_MAP;
		/*
		 * '+ 8' for 'del-map('
		 * '- 9' for 'del-map(' + trailing ')'
		 */
		rule->arg.map.ref = my_strndup(args[0] + 8, strlen(args[0]) - 9);

		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg+1] && strcmp(args[cur_arg+1], "if") != 0 && strcmp(args[cur_arg+1], "unless") != 0)) {
			ha_alert("parsing [%s:%d]: 'http-request %s' expects exactly 1 argument.\n",
				 file, linenum, args[0]);
			goto out_err;
		}

		LIST_INIT(&rule->arg.map.key);
		proxy->conf.args.ctx = ARGC_HRQ;
		error = NULL;
		if (!parse_logformat_string(args[cur_arg], proxy, &rule->arg.map.key, LOG_OPT_HTTP,
		                            (proxy->cap & PR_CAP_FE) ? SMP_VAL_FE_HRQ_HDR : SMP_VAL_BE_HRQ_HDR, &error)) {
			ha_alert("parsing [%s:%d]: 'http-request %s': %s.\n",
				 file, linenum, args[0], error);
			free(error);
			goto out_err;
		}
		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;
		cur_arg += 1;
	} else if (strncmp(args[0], "set-map", 7) == 0) {
		/* http-request set-map(<reference (map name)>) <key pattern> <value pattern> */
		rule->action = ACT_HTTP_SET_MAP;
		/*
		 * '+ 8' for 'set-map('
		 * '- 9' for 'set-map(' + trailing ')'
		 */
		rule->arg.map.ref = my_strndup(args[0] + 8, strlen(args[0]) - 9);

		cur_arg = 1;

		if (!*args[cur_arg] || !*args[cur_arg+1] ||
		    (*args[cur_arg+2] && strcmp(args[cur_arg+2], "if") != 0 && strcmp(args[cur_arg+2], "unless") != 0)) {
			ha_alert("parsing [%s:%d]: 'http-request %s' expects exactly 2 arguments.\n",
				 file, linenum, args[0]);
			goto out_err;
		}

		LIST_INIT(&rule->arg.map.key);
		LIST_INIT(&rule->arg.map.value);
		proxy->conf.args.ctx = ARGC_HRQ;

		/* key pattern */
		error = NULL;
		if (!parse_logformat_string(args[cur_arg], proxy, &rule->arg.map.key, LOG_OPT_HTTP,
		                            (proxy->cap & PR_CAP_FE) ? SMP_VAL_FE_HRQ_HDR : SMP_VAL_BE_HRQ_HDR, &error)) {
			ha_alert("parsing [%s:%d]: 'http-request %s' key: %s.\n",
				 file, linenum, args[0], error);
			free(error);
			goto out_err;
		}

		/* value pattern */
		error = NULL;
		if (!parse_logformat_string(args[cur_arg + 1], proxy, &rule->arg.map.value, LOG_OPT_HTTP,
		                            (proxy->cap & PR_CAP_FE) ? SMP_VAL_FE_HRQ_HDR : SMP_VAL_BE_HRQ_HDR, &error)) {
			ha_alert("parsing [%s:%d]: 'http-request %s' pattern: %s.\n",
				 file, linenum, args[0], error);
			free(error);
			goto out_err;
		}
		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;

		cur_arg += 2;
	} else if (((custom = action_http_req_custom(args[0])) != NULL)) {
		char *errmsg = NULL;
		cur_arg = 1;
		/* try in the module list */
		rule->from = ACT_F_HTTP_REQ;
		rule->kw = custom;
		if (custom->parse(args, &cur_arg, proxy, rule, &errmsg) == ACT_RET_PRS_ERR) {
			ha_alert("parsing [%s:%d] : error detected in %s '%s' while parsing 'http-request %s' rule : %s.\n",
				 file, linenum, proxy_type_str(proxy), proxy->id, args[0], errmsg);
			free(errmsg);
			goto out_err;
		}
	} else {
		action_build_list(&http_req_keywords.list, &trash);
		ha_alert("parsing [%s:%d]: 'http-request' expects 'allow', 'deny', 'auth', 'redirect', "
			 "'tarpit', 'add-header', 'set-header', 'replace-header', 'replace-value', 'set-nice', "
			 "'set-tos', 'set-mark', 'set-log-level', 'add-acl', 'del-acl', 'del-map', 'set-map', 'track-sc*'"
			 "%s%s, but got '%s'%s.\n",
			 file, linenum, *trash.str ? ", " : "", trash.str, args[0], *args[0] ? "" : " (missing argument)");
		goto out_err;
	}

	if (strcmp(args[cur_arg], "if") == 0 || strcmp(args[cur_arg], "unless") == 0) {
		struct acl_cond *cond;
		char *errmsg = NULL;

		if ((cond = build_acl_cond(file, linenum, &proxy->acl, proxy, args+cur_arg, &errmsg)) == NULL) {
			ha_alert("parsing [%s:%d] : error detected while parsing an 'http-request %s' condition : %s.\n",
				 file, linenum, args[0], errmsg);
			free(errmsg);
			goto out_err;
		}
		rule->cond = cond;
	}
	else if (*args[cur_arg]) {
		ha_alert("parsing [%s:%d]: 'http-request %s' expects 'realm' for 'auth' or"
			 " either 'if' or 'unless' followed by a condition but found '%s'.\n",
			 file, linenum, args[0], args[cur_arg]);
		goto out_err;
	}

	return rule;
 out_err:
	free(rule);
	return NULL;
}
