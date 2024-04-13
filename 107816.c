static void push_param(HashTable *params, php_http_params_state_t *state, const php_http_params_opts_t *opts TSRMLS_DC)
{
	if (state->val.str) {
		if (!state->current.val) {
			return;
		} else if (0 < (state->val.len = state->input.str - state->val.str)) {
			sanitize_value(opts->flags, state->val.str, state->val.len, *(state->current.val), state->rfc5987 TSRMLS_CC);
		} else {
			ZVAL_EMPTY_STRING(*(state->current.val));
		}
		state->rfc5987 = 0;
	} else if (state->arg.str) {
		if (0 < (state->arg.len = state->input.str - state->arg.str)) {
			zval *val, key;
			zend_bool rfc5987 = 0;

			INIT_PZVAL(&key);
			ZVAL_NULL(&key);
			sanitize_key(opts->flags, state->arg.str, state->arg.len, &key, &rfc5987 TSRMLS_CC);
			state->rfc5987 = rfc5987;
			if (Z_TYPE(key) == IS_STRING && Z_STRLEN(key)) {
				MAKE_STD_ZVAL(val);
				ZVAL_TRUE(val);

				if (rfc5987) {
					zval **rfc;

					if (SUCCESS == zend_hash_find(Z_ARRVAL_PP(state->current.args), ZEND_STRS("*rfc5987*"), (void *) &rfc)) {
						zend_symtable_update(Z_ARRVAL_PP(rfc), Z_STRVAL(key), Z_STRLEN(key) + 1, (void *) &val, sizeof(zval *), (void *) &state->current.val);
					} else {
						zval *tmp;

						MAKE_STD_ZVAL(tmp);
						array_init_size(tmp, 1);
						zend_symtable_update(Z_ARRVAL_P(tmp), Z_STRVAL(key), Z_STRLEN(key) + 1, (void *) &val, sizeof(zval *), (void *) &state->current.val);
						zend_symtable_update(Z_ARRVAL_PP(state->current.args), ZEND_STRS("*rfc5987*"), (void *) &tmp, sizeof(zval *), NULL);
					}
				} else {
					zend_symtable_update(Z_ARRVAL_PP(state->current.args), Z_STRVAL(key), Z_STRLEN(key) + 1, (void *) &val, sizeof(zval *), (void *) &state->current.val);
				}
			}
			zval_dtor(&key);
		}
	} else if (state->param.str) {
		if (0 < (state->param.len = state->input.str - state->param.str)) {
			zval *prm, *arg, *val, *key;
			zend_bool rfc5987 = 0;

			MAKE_STD_ZVAL(key);
			ZVAL_NULL(key);
			if (opts->flags & PHP_HTTP_PARAMS_RFC5988) {
				sanitize_rfc5988(state->param.str, state->param.len, key TSRMLS_CC);
			} else {
				sanitize_key(opts->flags, state->param.str, state->param.len, key, &rfc5987 TSRMLS_CC);
				state->rfc5987 = rfc5987;
			}
			if (Z_TYPE_P(key) != IS_STRING) {
				merge_param(params, key, &state->current.val, &state->current.args TSRMLS_CC);
			} else if (Z_STRLEN_P(key)) {
				MAKE_STD_ZVAL(prm);
				array_init_size(prm, 2);

				MAKE_STD_ZVAL(val);
				if (opts->defval) {
					ZVAL_COPY_VALUE(val, opts->defval);
					zval_copy_ctor(val);
				} else {
					ZVAL_TRUE(val);
				}
				if (rfc5987 && (opts->flags & PHP_HTTP_PARAMS_RFC5987)) {
					zend_hash_update(Z_ARRVAL_P(prm), "*rfc5987*", sizeof("*rfc5987*"), (void *) &val, sizeof(zval *), (void *) &state->current.val);
				} else {
					zend_hash_update(Z_ARRVAL_P(prm), "value", sizeof("value"), (void *) &val, sizeof(zval *), (void *) &state->current.val);
				}

				MAKE_STD_ZVAL(arg);
				array_init_size(arg, 3);
				zend_hash_update(Z_ARRVAL_P(prm), "arguments", sizeof("arguments"), (void *) &arg, sizeof(zval *), (void *) &state->current.args);

				zend_symtable_update(params, Z_STRVAL_P(key), Z_STRLEN_P(key) + 1, (void *) &prm, sizeof(zval *), (void *) &state->current.param);
			}
			zval_ptr_dtor(&key);
		}
	}
}
