static void init_request_info(fcgi_request *request TSRMLS_DC)
{
	char *env_script_filename = CGI_GETENV("SCRIPT_FILENAME");
	char *env_path_translated = CGI_GETENV("PATH_TRANSLATED");
	char *script_path_translated = env_script_filename;

	/* some broken servers do not have script_filename or argv0
	 * an example, IIS configured in some ways.  then they do more
	 * broken stuff and set path_translated to the cgi script location */
	if (!script_path_translated && env_path_translated) {
		script_path_translated = env_path_translated;
	}

	/* initialize the defaults */
	SG(request_info).path_translated = NULL;
	SG(request_info).request_method = NULL;
	SG(request_info).proto_num = 1000;
	SG(request_info).query_string = NULL;
	SG(request_info).request_uri = NULL;
	SG(request_info).content_type = NULL;
	SG(request_info).content_length = 0;
	SG(sapi_headers).http_response_code = 200;

	/* script_path_translated being set is a good indication that
	 * we are running in a cgi environment, since it is always
	 * null otherwise.  otherwise, the filename
	 * of the script will be retreived later via argc/argv */
	if (script_path_translated) {
		const char *auth;
		char *content_length = CGI_GETENV("CONTENT_LENGTH");
		char *content_type = CGI_GETENV("CONTENT_TYPE");
		char *env_path_info = CGI_GETENV("PATH_INFO");
		char *env_script_name = CGI_GETENV("SCRIPT_NAME");

#ifdef PHP_WIN32
		/* Hack for buggy IIS that sets incorrect PATH_INFO */
		char *env_server_software = CGI_GETENV("SERVER_SOFTWARE");

		if (env_server_software &&
			env_script_name &&
			env_path_info &&
			strncmp(env_server_software, "Microsoft-IIS", sizeof("Microsoft-IIS")-1) == 0 &&
			strncmp(env_path_info, env_script_name, strlen(env_script_name)) == 0
		) {
			env_path_info = CGI_PUTENV("ORIG_PATH_INFO", env_path_info);
			env_path_info += strlen(env_script_name);
			if (*env_path_info == 0) {
				env_path_info = NULL;
			}
			env_path_info = CGI_PUTENV("PATH_INFO", env_path_info);
		}
#endif

		if (CGIG(fix_pathinfo)) {
			struct stat st;
			char *real_path = NULL;
			char *env_redirect_url = CGI_GETENV("REDIRECT_URL");
			char *env_document_root = CGI_GETENV("DOCUMENT_ROOT");
			char *orig_path_translated = env_path_translated;
			char *orig_path_info = env_path_info;
			char *orig_script_name = env_script_name;
			char *orig_script_filename = env_script_filename;
			int script_path_translated_len;

			if (!env_document_root && PG(doc_root)) {
				env_document_root = CGI_PUTENV("DOCUMENT_ROOT", PG(doc_root));
				/* fix docroot */
				TRANSLATE_SLASHES(env_document_root);
			}

			if (env_path_translated != NULL && env_redirect_url != NULL &&
 			    env_path_translated != script_path_translated &&
 			    strcmp(env_path_translated, script_path_translated) != 0) {
				/*
				 * pretty much apache specific.  If we have a redirect_url
				 * then our script_filename and script_name point to the
				 * php executable
				 */
				script_path_translated = env_path_translated;
				/* we correct SCRIPT_NAME now in case we don't have PATH_INFO */
				env_script_name = env_redirect_url;
			}

#ifdef __riscos__
			/* Convert path to unix format*/
			__riscosify_control |= __RISCOSIFY_DONT_CHECK_DIR;
			script_path_translated = __unixify(script_path_translated, 0, NULL, 1, 0);
#endif

			/*
			 * if the file doesn't exist, try to extract PATH_INFO out
			 * of it by stat'ing back through the '/'
			 * this fixes url's like /info.php/test
			 */
			if (script_path_translated &&
				(script_path_translated_len = strlen(script_path_translated)) > 0 &&
				(script_path_translated[script_path_translated_len-1] == '/' ||
#ifdef PHP_WIN32
				script_path_translated[script_path_translated_len-1] == '\\' ||
#endif
				(real_path = tsrm_realpath(script_path_translated, NULL TSRMLS_CC)) == NULL)
			) {
				char *pt = estrndup(script_path_translated, script_path_translated_len);
				int len = script_path_translated_len;
				char *ptr;

				while ((ptr = strrchr(pt, '/')) || (ptr = strrchr(pt, '\\'))) {
					*ptr = 0;
					if (stat(pt, &st) == 0 && S_ISREG(st.st_mode)) {
						/*
						 * okay, we found the base script!
						 * work out how many chars we had to strip off;
						 * then we can modify PATH_INFO
						 * accordingly
						 *
						 * we now have the makings of
						 * PATH_INFO=/test
						 * SCRIPT_FILENAME=/docroot/info.php
						 *
						 * we now need to figure out what docroot is.
						 * if DOCUMENT_ROOT is set, this is easy, otherwise,
						 * we have to play the game of hide and seek to figure
						 * out what SCRIPT_NAME should be
						 */
						int slen = len - strlen(pt);
						int pilen = env_path_info ? strlen(env_path_info) : 0;
						char *path_info = env_path_info ? env_path_info + pilen - slen : NULL;

						if (orig_path_info != path_info) {
							if (orig_path_info) {
								char old;

								CGI_PUTENV("ORIG_PATH_INFO", orig_path_info);
								old = path_info[0];
								path_info[0] = 0;
								if (!orig_script_name ||
									strcmp(orig_script_name, env_path_info) != 0) {
									if (orig_script_name) {
										CGI_PUTENV("ORIG_SCRIPT_NAME", orig_script_name);
									}
									SG(request_info).request_uri = CGI_PUTENV("SCRIPT_NAME", env_path_info);
								} else {
									SG(request_info).request_uri = orig_script_name;
								}
								path_info[0] = old;
							}
							env_path_info = CGI_PUTENV("PATH_INFO", path_info);
						}
						if (!orig_script_filename ||
							strcmp(orig_script_filename, pt) != 0) {
							if (orig_script_filename) {
								CGI_PUTENV("ORIG_SCRIPT_FILENAME", orig_script_filename);
							}
							script_path_translated = CGI_PUTENV("SCRIPT_FILENAME", pt);
						}
						TRANSLATE_SLASHES(pt);

						/* figure out docroot
						 * SCRIPT_FILENAME minus SCRIPT_NAME
						 */
						if (env_document_root) {
							int l = strlen(env_document_root);
							int path_translated_len = 0;
							char *path_translated = NULL;

							if (l && env_document_root[l - 1] == '/') {
								--l;
							}

							/* we have docroot, so we should have:
							 * DOCUMENT_ROOT=/docroot
							 * SCRIPT_FILENAME=/docroot/info.php
							 */

							/* PATH_TRANSLATED = DOCUMENT_ROOT + PATH_INFO */
							path_translated_len = l + (env_path_info ? strlen(env_path_info) : 0);
							path_translated = (char *) emalloc(path_translated_len + 1);
							memcpy(path_translated, env_document_root, l);
							if (env_path_info) {
								memcpy(path_translated + l, env_path_info, (path_translated_len - l));
							}
							path_translated[path_translated_len] = '\0';
							if (orig_path_translated) {
								CGI_PUTENV("ORIG_PATH_TRANSLATED", orig_path_translated);
							}
							env_path_translated = CGI_PUTENV("PATH_TRANSLATED", path_translated);
							efree(path_translated);
						} else if (	env_script_name &&
									strstr(pt, env_script_name)
						) {
							/* PATH_TRANSLATED = PATH_TRANSLATED - SCRIPT_NAME + PATH_INFO */
							int ptlen = strlen(pt) - strlen(env_script_name);
							int path_translated_len = ptlen + (env_path_info ? strlen(env_path_info) : 0);
							char *path_translated = NULL;

							path_translated = (char *) emalloc(path_translated_len + 1);
							memcpy(path_translated, pt, ptlen);
							if (env_path_info) {
								memcpy(path_translated + ptlen, env_path_info, path_translated_len - ptlen);
							}
							path_translated[path_translated_len] = '\0';
							if (orig_path_translated) {
								CGI_PUTENV("ORIG_PATH_TRANSLATED", orig_path_translated);
							}
							env_path_translated = CGI_PUTENV("PATH_TRANSLATED", path_translated);
							efree(path_translated);
						}
						break;
					}
				}
				if (!ptr) {
					/*
					 * if we stripped out all the '/' and still didn't find
					 * a valid path... we will fail, badly. of course we would
					 * have failed anyway... we output 'no input file' now.
					 */
					if (orig_script_filename) {
						CGI_PUTENV("ORIG_SCRIPT_FILENAME", orig_script_filename);
					}
					script_path_translated = CGI_PUTENV("SCRIPT_FILENAME", NULL);
					SG(sapi_headers).http_response_code = 404;
				}
				if (!SG(request_info).request_uri) {
					if (!orig_script_name ||
						strcmp(orig_script_name, env_script_name) != 0) {
						if (orig_script_name) {
							CGI_PUTENV("ORIG_SCRIPT_NAME", orig_script_name);
						}
						SG(request_info).request_uri = CGI_PUTENV("SCRIPT_NAME", env_script_name);
					} else {
						SG(request_info).request_uri = orig_script_name;
					}
				}
				if (pt) {
					efree(pt);
				}
			} else {
				/* make sure path_info/translated are empty */
				if (!orig_script_filename ||
					(script_path_translated != orig_script_filename &&
					strcmp(script_path_translated, orig_script_filename) != 0)) {
					if (orig_script_filename) {
						CGI_PUTENV("ORIG_SCRIPT_FILENAME", orig_script_filename);
					}
					script_path_translated = CGI_PUTENV("SCRIPT_FILENAME", script_path_translated);
				}
				if (env_redirect_url) {
					if (orig_path_info) {
						CGI_PUTENV("ORIG_PATH_INFO", orig_path_info);
						CGI_PUTENV("PATH_INFO", NULL);
					}
					if (orig_path_translated) {
						CGI_PUTENV("ORIG_PATH_TRANSLATED", orig_path_translated);
						CGI_PUTENV("PATH_TRANSLATED", NULL);
					}
				}
				if (env_script_name != orig_script_name) {
					if (orig_script_name) {
						CGI_PUTENV("ORIG_SCRIPT_NAME", orig_script_name);
					}
					SG(request_info).request_uri = CGI_PUTENV("SCRIPT_NAME", env_script_name);
				} else {
					SG(request_info).request_uri = env_script_name;
				}
				free(real_path);
			}
		} else {
			/* pre 4.3 behaviour, shouldn't be used but provides BC */
			if (env_path_info) {
				SG(request_info).request_uri = env_path_info;
			} else {
				SG(request_info).request_uri = env_script_name;
			}
			if (!CGIG(discard_path) && env_path_translated) {
				script_path_translated = env_path_translated;
			}
		}

		if (is_valid_path(script_path_translated)) {
			SG(request_info).path_translated = estrdup(script_path_translated);
		}

		SG(request_info).request_method = CGI_GETENV("REQUEST_METHOD");
		/* FIXME - Work out proto_num here */
		SG(request_info).query_string = CGI_GETENV("QUERY_STRING");
		SG(request_info).content_type = (content_type ? content_type : "" );
		SG(request_info).content_length = (content_length ? atol(content_length) : 0);

		/* The CGI RFC allows servers to pass on unvalidated Authorization data */
		auth = CGI_GETENV("HTTP_AUTHORIZATION");
		php_handle_auth_data(auth TSRMLS_CC);
	}
}
