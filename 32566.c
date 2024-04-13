struct http_res_rule *parse_http_res_cond(const char **args, const char *file, int linenum, struct proxy *proxy)
{
	struct http_res_rule *rule;
	struct http_res_action_kw *custom = NULL;
	int cur_arg;
	char *error;

	rule = calloc(1, sizeof(*rule));
	if (!rule) {
		Alert("parsing [%s:%d]: out of memory.\n", file, linenum);
		goto out_err;
	}

	if (!strcmp(args[0], "allow")) {
		rule->action = HTTP_RES_ACT_ALLOW;
		cur_arg = 1;
	} else if (!strcmp(args[0], "deny")) {
		rule->action = HTTP_RES_ACT_DENY;
		cur_arg = 1;
	} else if (!strcmp(args[0], "set-nice")) {
		rule->action = HTTP_RES_ACT_SET_NICE;
		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg + 1] && strcmp(args[cur_arg + 1], "if") != 0 && strcmp(args[cur_arg + 1], "unless") != 0)) {
			Alert("parsing [%s:%d]: 'http-response %s' expects exactly 1 argument (integer value).\n",
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
		rule->action = HTTP_RES_ACT_SET_TOS;
		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg + 1] && strcmp(args[cur_arg + 1], "if") != 0 && strcmp(args[cur_arg + 1], "unless") != 0)) {
			Alert("parsing [%s:%d]: 'http-response %s' expects exactly 1 argument (integer/hex value).\n",
			      file, linenum, args[0]);
			goto out_err;
		}

		rule->arg.tos = strtol(args[cur_arg], &err, 0);
		if (err && *err != '\0') {
			Alert("parsing [%s:%d]: invalid character starting at '%s' in 'http-response %s' (integer/hex value expected).\n",
			      file, linenum, err, args[0]);
			goto out_err;
		}
		cur_arg++;
#else
		Alert("parsing [%s:%d]: 'http-response %s' is not supported on this platform (IP_TOS undefined).\n", file, linenum, args[0]);
		goto out_err;
#endif
	} else if (!strcmp(args[0], "set-mark")) {
#ifdef SO_MARK
		char *err;
		rule->action = HTTP_RES_ACT_SET_MARK;
		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg + 1] && strcmp(args[cur_arg + 1], "if") != 0 && strcmp(args[cur_arg + 1], "unless") != 0)) {
			Alert("parsing [%s:%d]: 'http-response %s' expects exactly 1 argument (integer/hex value).\n",
			      file, linenum, args[0]);
			goto out_err;
		}

		rule->arg.mark = strtoul(args[cur_arg], &err, 0);
		if (err && *err != '\0') {
			Alert("parsing [%s:%d]: invalid character starting at '%s' in 'http-response %s' (integer/hex value expected).\n",
			      file, linenum, err, args[0]);
			goto out_err;
		}
		cur_arg++;
		global.last_checks |= LSTCHK_NETADM;
#else
		Alert("parsing [%s:%d]: 'http-response %s' is not supported on this platform (SO_MARK undefined).\n", file, linenum, args[0]);
		goto out_err;
#endif
	} else if (!strcmp(args[0], "set-log-level")) {
		rule->action = HTTP_RES_ACT_SET_LOGL;
		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg + 1] && strcmp(args[cur_arg + 1], "if") != 0 && strcmp(args[cur_arg + 1], "unless") != 0)) {
		bad_log_level:
			Alert("parsing [%s:%d]: 'http-response %s' expects exactly 1 argument (log level name or 'silent').\n",
			      file, linenum, args[0]);
			goto out_err;
		}
		if (strcmp(args[cur_arg], "silent") == 0)
			rule->arg.loglevel = -1;
		else if ((rule->arg.loglevel = get_log_level(args[cur_arg] + 1)) == 0)
			goto bad_log_level;
		cur_arg++;
	} else if (strcmp(args[0], "add-header") == 0 || strcmp(args[0], "set-header") == 0) {
		rule->action = *args[0] == 'a' ? HTTP_RES_ACT_ADD_HDR : HTTP_RES_ACT_SET_HDR;
		cur_arg = 1;

		if (!*args[cur_arg] || !*args[cur_arg+1] ||
		    (*args[cur_arg+2] && strcmp(args[cur_arg+2], "if") != 0 && strcmp(args[cur_arg+2], "unless") != 0)) {
			Alert("parsing [%s:%d]: 'http-response %s' expects exactly 2 arguments.\n",
			      file, linenum, args[0]);
			goto out_err;
		}

		rule->arg.hdr_add.name = strdup(args[cur_arg]);
		rule->arg.hdr_add.name_len = strlen(rule->arg.hdr_add.name);
		LIST_INIT(&rule->arg.hdr_add.fmt);

		proxy->conf.args.ctx = ARGC_HRS;
		parse_logformat_string(args[cur_arg + 1], proxy, &rule->arg.hdr_add.fmt, LOG_OPT_HTTP,
				       (proxy->cap & PR_CAP_BE) ? SMP_VAL_BE_HRS_HDR : SMP_VAL_FE_HRS_HDR,
				       file, linenum);
		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;
		cur_arg += 2;
	} else if (strcmp(args[0], "replace-header") == 0 || strcmp(args[0], "replace-value") == 0) {
		rule->action = args[0][8] == 'h' ? HTTP_RES_ACT_REPLACE_HDR : HTTP_RES_ACT_REPLACE_VAL;
		cur_arg = 1;

		if (!*args[cur_arg] || !*args[cur_arg+1] || !*args[cur_arg+2] ||
		    (*args[cur_arg+3] && strcmp(args[cur_arg+3], "if") != 0 && strcmp(args[cur_arg+3], "unless") != 0)) {
			Alert("parsing [%s:%d]: 'http-response %s' expects exactly 3 arguments.\n",
			      file, linenum, args[0]);
			goto out_err;
		}

		rule->arg.hdr_add.name = strdup(args[cur_arg]);
		rule->arg.hdr_add.name_len = strlen(rule->arg.hdr_add.name);
		LIST_INIT(&rule->arg.hdr_add.fmt);

		error = NULL;
		if (!regex_comp(args[cur_arg + 1], &rule->arg.hdr_add.re, 1, 1, &error)) {
			Alert("parsing [%s:%d] : '%s' : %s.\n", file, linenum,
			      args[cur_arg + 1], error);
			free(error);
			goto out_err;
		}

		proxy->conf.args.ctx = ARGC_HRQ;
		parse_logformat_string(args[cur_arg + 2], proxy, &rule->arg.hdr_add.fmt, LOG_OPT_HTTP,
				       (proxy->cap & PR_CAP_BE) ? SMP_VAL_BE_HRS_HDR : SMP_VAL_FE_HRS_HDR,
				       file, linenum);

		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;
		cur_arg += 3;
	} else if (strcmp(args[0], "del-header") == 0) {
		rule->action = HTTP_RES_ACT_DEL_HDR;
		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg+1] && strcmp(args[cur_arg+1], "if") != 0 && strcmp(args[cur_arg+1], "unless") != 0)) {
			Alert("parsing [%s:%d]: 'http-response %s' expects exactly 1 argument.\n",
			      file, linenum, args[0]);
			goto out_err;
		}

		rule->arg.hdr_add.name = strdup(args[cur_arg]);
		rule->arg.hdr_add.name_len = strlen(rule->arg.hdr_add.name);

		proxy->conf.args.ctx = ARGC_HRS;
		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;
		cur_arg += 1;
	} else if (strncmp(args[0], "add-acl", 7) == 0) {
		/* http-request add-acl(<reference (acl name)>) <key pattern> */
		rule->action = HTTP_RES_ACT_ADD_ACL;
		/*
		 * '+ 8' for 'add-acl('
		 * '- 9' for 'add-acl(' + trailing ')'
		 */
		rule->arg.map.ref = my_strndup(args[0] + 8, strlen(args[0]) - 9);

		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg+1] && strcmp(args[cur_arg+1], "if") != 0 && strcmp(args[cur_arg+1], "unless") != 0)) {
			Alert("parsing [%s:%d]: 'http-response %s' expects exactly 1 argument.\n",
			      file, linenum, args[0]);
			goto out_err;
		}

		LIST_INIT(&rule->arg.map.key);
		proxy->conf.args.ctx = ARGC_HRS;
		parse_logformat_string(args[cur_arg], proxy, &rule->arg.map.key, LOG_OPT_HTTP,
			(proxy->cap & PR_CAP_BE) ? SMP_VAL_BE_HRS_HDR : SMP_VAL_FE_HRS_HDR,
			file, linenum);
		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;

		cur_arg += 1;
	} else if (strncmp(args[0], "del-acl", 7) == 0) {
		/* http-response del-acl(<reference (acl name)>) <key pattern> */
		rule->action = HTTP_RES_ACT_DEL_ACL;
		/*
		 * '+ 8' for 'del-acl('
		 * '- 9' for 'del-acl(' + trailing ')'
		 */
		rule->arg.map.ref = my_strndup(args[0] + 8, strlen(args[0]) - 9);

		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg+1] && strcmp(args[cur_arg+1], "if") != 0 && strcmp(args[cur_arg+1], "unless") != 0)) {
			Alert("parsing [%s:%d]: 'http-response %s' expects exactly 1 argument.\n",
			      file, linenum, args[0]);
			goto out_err;
		}

		LIST_INIT(&rule->arg.map.key);
		proxy->conf.args.ctx = ARGC_HRS;
		parse_logformat_string(args[cur_arg], proxy, &rule->arg.map.key, LOG_OPT_HTTP,
			(proxy->cap & PR_CAP_BE) ? SMP_VAL_BE_HRS_HDR : SMP_VAL_FE_HRS_HDR,
			file, linenum);
		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;
		cur_arg += 1;
	} else if (strncmp(args[0], "del-map", 7) == 0) {
		/* http-response del-map(<reference (map name)>) <key pattern> */
		rule->action = HTTP_RES_ACT_DEL_MAP;
		/*
		 * '+ 8' for 'del-map('
		 * '- 9' for 'del-map(' + trailing ')'
		 */
		rule->arg.map.ref = my_strndup(args[0] + 8, strlen(args[0]) - 9);

		cur_arg = 1;

		if (!*args[cur_arg] ||
		    (*args[cur_arg+1] && strcmp(args[cur_arg+1], "if") != 0 && strcmp(args[cur_arg+1], "unless") != 0)) {
			Alert("parsing [%s:%d]: 'http-response %s' expects exactly 1 argument.\n",
			      file, linenum, args[0]);
			goto out_err;
		}

		LIST_INIT(&rule->arg.map.key);
		proxy->conf.args.ctx = ARGC_HRS;
		parse_logformat_string(args[cur_arg], proxy, &rule->arg.map.key, LOG_OPT_HTTP,
			(proxy->cap & PR_CAP_BE) ? SMP_VAL_BE_HRS_HDR : SMP_VAL_FE_HRS_HDR,
			file, linenum);
		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;
		cur_arg += 1;
	} else if (strncmp(args[0], "set-map", 7) == 0) {
		/* http-response set-map(<reference (map name)>) <key pattern> <value pattern> */
		rule->action = HTTP_RES_ACT_SET_MAP;
		/*
		 * '+ 8' for 'set-map('
		 * '- 9' for 'set-map(' + trailing ')'
		 */
		rule->arg.map.ref = my_strndup(args[0] + 8, strlen(args[0]) - 9);

		cur_arg = 1;

		if (!*args[cur_arg] || !*args[cur_arg+1] ||
		    (*args[cur_arg+2] && strcmp(args[cur_arg+2], "if") != 0 && strcmp(args[cur_arg+2], "unless") != 0)) {
			Alert("parsing [%s:%d]: 'http-response %s' expects exactly 2 arguments.\n",
			      file, linenum, args[0]);
			goto out_err;
		}

		LIST_INIT(&rule->arg.map.key);
		LIST_INIT(&rule->arg.map.value);

		proxy->conf.args.ctx = ARGC_HRS;

		/* key pattern */
		parse_logformat_string(args[cur_arg], proxy, &rule->arg.map.key, LOG_OPT_HTTP,
			(proxy->cap & PR_CAP_BE) ? SMP_VAL_BE_HRS_HDR : SMP_VAL_FE_HRS_HDR,
			file, linenum);

		/* value pattern */
		parse_logformat_string(args[cur_arg + 1], proxy, &rule->arg.map.value, LOG_OPT_HTTP,
			(proxy->cap & PR_CAP_BE) ? SMP_VAL_BE_HRS_HDR : SMP_VAL_FE_HRS_HDR,
			file, linenum);

		free(proxy->conf.lfs_file);
		proxy->conf.lfs_file = strdup(proxy->conf.args.file);
		proxy->conf.lfs_line = proxy->conf.args.line;

		cur_arg += 2;
	} else if (((custom = action_http_res_custom(args[0])) != NULL)) {
		char *errmsg = NULL;
		cur_arg = 1;
		/* try in the module list */
		if (custom->parse(args, &cur_arg, proxy, rule, &errmsg) < 0) {
			Alert("parsing [%s:%d] : error detected in %s '%s' while parsing 'http-response %s' rule : %s.\n",
			      file, linenum, proxy_type_str(proxy), proxy->id, args[0], errmsg);
			free(errmsg);
			goto out_err;
		}
	} else {
		Alert("parsing [%s:%d]: 'http-response' expects 'allow', 'deny', 'redirect', 'add-header', 'del-header', 'set-header', 'replace-header', 'replace-value', 'set-nice', 'set-tos', 'set-mark', 'set-log-level', 'del-acl', 'add-acl', 'del-map', 'set-map', but got '%s'%s.\n",
		      file, linenum, args[0], *args[0] ? "" : " (missing argument)");
		goto out_err;
	}

	if (strcmp(args[cur_arg], "if") == 0 || strcmp(args[cur_arg], "unless") == 0) {
		struct acl_cond *cond;
		char *errmsg = NULL;

		if ((cond = build_acl_cond(file, linenum, proxy, args+cur_arg, &errmsg)) == NULL) {
			Alert("parsing [%s:%d] : error detected while parsing an 'http-response %s' condition : %s.\n",
			      file, linenum, args[0], errmsg);
			free(errmsg);
			goto out_err;
		}
		rule->cond = cond;
	}
	else if (*args[cur_arg]) {
		Alert("parsing [%s:%d]: 'http-response %s' expects"
		      " either 'if' or 'unless' followed by a condition but found '%s'.\n",
		      file, linenum, args[0], args[cur_arg]);
		goto out_err;
	}

	return rule;
 out_err:
	free(rule);
	return NULL;
}
