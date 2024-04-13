PHP_FUNCTION(getopt)
{
	char *options = NULL, **argv = NULL;
	char opt[2] = { '\0' };
	char *optname;
	int argc = 0, options_len = 0, len, o;
	char *php_optarg = NULL;
	int php_optind = 1;
	zval *val, **args = NULL, *p_longopts = NULL;
	int optname_len = 0;
	opt_struct *opts, *orig_opts;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|a", &options, &options_len, &p_longopts) == FAILURE) {
		RETURN_FALSE;
	}

	/* Get argv from the global symbol table. We calculate argc ourselves
	 * in order to be on the safe side, even though it is also available
	 * from the symbol table. */
	if (PG(http_globals)[TRACK_VARS_SERVER] &&
		(zend_hash_find(HASH_OF(PG(http_globals)[TRACK_VARS_SERVER]), "argv", sizeof("argv"), (void **) &args) != FAILURE ||
		zend_hash_find(&EG(symbol_table), "argv", sizeof("argv"), (void **) &args) != FAILURE) && Z_TYPE_PP(args) == IS_ARRAY
	) {
		int pos = 0;
		zval **entry;

		argc = zend_hash_num_elements(Z_ARRVAL_PP(args));

		/* Attempt to allocate enough memory to hold all of the arguments
		 * and a trailing NULL */
		argv = (char **) safe_emalloc(sizeof(char *), (argc + 1), 0);

		/* Reset the array indexes. */
		zend_hash_internal_pointer_reset(Z_ARRVAL_PP(args));

		/* Iterate over the hash to construct the argv array. */
		while (zend_hash_get_current_data(Z_ARRVAL_PP(args), (void **)&entry) == SUCCESS) {
			zval arg, *arg_ptr = *entry;

			if (Z_TYPE_PP(entry) != IS_STRING) {
				arg = **entry;
				zval_copy_ctor(&arg);
				convert_to_string(&arg);
				arg_ptr = &arg;
			}

			argv[pos++] = estrdup(Z_STRVAL_P(arg_ptr));

			if (arg_ptr != *entry) {
				zval_dtor(&arg);
			}

			zend_hash_move_forward(Z_ARRVAL_PP(args));
		}

		/* The C Standard requires argv[argc] to be NULL - this might
		 * keep some getopt implementations happy. */
		argv[argc] = NULL;
	} else {
		/* Return false if we can't find argv. */
		RETURN_FALSE;
	}

	len = parse_opts(options, &opts);

	if (p_longopts) {
		int count;
		zval **entry;

		count = zend_hash_num_elements(Z_ARRVAL_P(p_longopts));

		/* the first <len> slots are filled by the one short ops
		 * we now extend our array and jump to the new added structs */
		opts = (opt_struct *) erealloc(opts, sizeof(opt_struct) * (len + count + 1));
		orig_opts = opts;
		opts += len;

		memset(opts, 0, count * sizeof(opt_struct));

		/* Reset the array indexes. */
		zend_hash_internal_pointer_reset(Z_ARRVAL_P(p_longopts));

		/* Iterate over the hash to construct the argv array. */
		while (zend_hash_get_current_data(Z_ARRVAL_P(p_longopts), (void **)&entry) == SUCCESS) {
			zval arg, *arg_ptr = *entry;

			if (Z_TYPE_PP(entry) != IS_STRING) {
				arg = **entry;
				zval_copy_ctor(&arg);
				convert_to_string(&arg);
				arg_ptr = &arg;
			}

			opts->need_param = 0;
			opts->opt_name = estrdup(Z_STRVAL_P(arg_ptr));
			len = strlen(opts->opt_name);
			if ((len > 0) && (opts->opt_name[len - 1] == ':')) {
				opts->need_param++;
				opts->opt_name[len - 1] = '\0';
				if ((len > 1) && (opts->opt_name[len - 2] == ':')) {
					opts->need_param++;
					opts->opt_name[len - 2] = '\0';
				}
			}
			opts->opt_char = 0;
			opts++;

			if (arg_ptr != *entry) {
				zval_dtor(&arg);
			}

			zend_hash_move_forward(Z_ARRVAL_P(p_longopts));
		}
	} else {
		opts = (opt_struct*) erealloc(opts, sizeof(opt_struct) * (len + 1));
		orig_opts = opts;
		opts += len;
	}

	/* php_getopt want to identify the last param */
	opts->opt_char   = '-';
	opts->need_param = 0;
	opts->opt_name   = NULL;

	/* Initialize the return value as an array. */
	array_init(return_value);

	/* after our pointer arithmetic jump back to the first element */
	opts = orig_opts;

	while ((o = php_getopt(argc, argv, opts, &php_optarg, &php_optind, 0, 1)) != -1) {
		/* Skip unknown arguments. */
		if (o == '?') {
			continue;
		}

		/* Prepare the option character and the argument string. */
		if (o == 0) {
			optname = opts[php_optidx].opt_name;
		} else {
			if (o == 1) {
				o = '-';
			}
			opt[0] = o;
			optname = opt;
		}

		MAKE_STD_ZVAL(val);
		if (php_optarg != NULL) {
			/* keep the arg as binary, since the encoding is not known */
			ZVAL_STRING(val, php_optarg, 1);
		} else {
			ZVAL_FALSE(val);
		}

		/* Add this option / argument pair to the result hash. */
		optname_len = strlen(optname);
		if (!(optname_len > 1 && optname[0] == '0') && is_numeric_string(optname, optname_len, NULL, NULL, 0) == IS_LONG) {
			/* numeric string */
			int optname_int = atoi(optname);
			if (zend_hash_index_find(HASH_OF(return_value), optname_int, (void **)&args) != FAILURE) {
				if (Z_TYPE_PP(args) != IS_ARRAY) {
					convert_to_array_ex(args);
				}
				zend_hash_next_index_insert(HASH_OF(*args), (void *)&val, sizeof(zval *), NULL);
			} else {
				zend_hash_index_update(HASH_OF(return_value), optname_int, &val, sizeof(zval *), NULL);
			}
		} else {
			/* other strings */
			if (zend_hash_find(HASH_OF(return_value), optname, strlen(optname)+1, (void **)&args) != FAILURE) {
				if (Z_TYPE_PP(args) != IS_ARRAY) {
					convert_to_array_ex(args);
				}
				zend_hash_next_index_insert(HASH_OF(*args), (void *)&val, sizeof(zval *), NULL);
			} else {
				zend_hash_add(HASH_OF(return_value), optname, strlen(optname)+1, (void *)&val, sizeof(zval *), NULL);
			}
		}

		php_optarg = NULL;
	}

	free_longopts(orig_opts);
	efree(orig_opts);
	free_argv(argv, argc);
}
