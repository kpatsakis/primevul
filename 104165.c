static void sapi_cgi_register_variables(zval *track_vars_array TSRMLS_DC)
{
	unsigned int php_self_len;
	char *php_self;

	/* In CGI mode, we consider the environment to be a part of the server
	 * variables
	 */
	php_import_environment_variables(track_vars_array TSRMLS_CC);

	if (CGIG(fix_pathinfo)) {
		char *script_name = SG(request_info).request_uri;
		char *path_info;
		int free_php_self;
		ALLOCA_FLAG(use_heap)

		if (fcgi_is_fastcgi()) {
			fcgi_request *request = (fcgi_request*) SG(server_context);

			path_info = FCGI_GETENV(request, "PATH_INFO");
		} else {
			path_info = getenv("PATH_INFO");
		}

		if (path_info) {
			unsigned int path_info_len = strlen(path_info);

			if (script_name) {
				unsigned int script_name_len = strlen(script_name);

				php_self_len = script_name_len + path_info_len;
				php_self = do_alloca(php_self_len + 1, use_heap);
				memcpy(php_self, script_name, script_name_len + 1);
				memcpy(php_self + script_name_len, path_info, path_info_len + 1);
				free_php_self = 1;
			}  else {
				php_self = path_info;
				php_self_len = path_info_len;
				free_php_self = 0;
			}
		} else if (script_name) {
			php_self = script_name;
			php_self_len = strlen(script_name);
			free_php_self = 0;
		} else {
			php_self = "";
			php_self_len = 0;
			free_php_self = 0;
		}

		/* Build the special-case PHP_SELF variable for the CGI version */
		if (sapi_module.input_filter(PARSE_SERVER, "PHP_SELF", &php_self, php_self_len, &php_self_len TSRMLS_CC)) {
			php_register_variable_safe("PHP_SELF", php_self, php_self_len, track_vars_array TSRMLS_CC);
		}
		if (free_php_self) {
			free_alloca(php_self, use_heap);
		}
	} else {
		php_self = SG(request_info).request_uri ? SG(request_info).request_uri : "";
		php_self_len = strlen(php_self);
		if (sapi_module.input_filter(PARSE_SERVER, "PHP_SELF", &php_self, php_self_len, &php_self_len TSRMLS_CC)) {
			php_register_variable_safe("PHP_SELF", php_self, php_self_len, track_vars_array TSRMLS_CC);
		}
	}
}
