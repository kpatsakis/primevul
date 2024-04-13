int main(int argc, char *argv[])
{
	int free_query_string = 0;
	int exit_status = SUCCESS;
	int cgi = 0, c, i, len;
	zend_file_handle file_handle;
	char *s;

	/* temporary locals */
	int behavior = PHP_MODE_STANDARD;
	int no_headers = 0;
	int orig_optind = php_optind;
	char *orig_optarg = php_optarg;
	char *script_file = NULL;
	int ini_entries_len = 0;
	/* end of temporary locals */

#ifdef ZTS
	void ***tsrm_ls;
#endif

	int max_requests = 500;
	int requests = 0;
	int fastcgi;
	char *bindpath = NULL;
	int fcgi_fd = 0;
	fcgi_request *request = NULL;
	int repeats = 1;
	int benchmark = 0;
#if HAVE_GETTIMEOFDAY
	struct timeval start, end;
#else
	time_t start, end;
#endif
#ifndef PHP_WIN32
	int status = 0;
#endif
	char *query_string;
	char *decoded_query_string;
	int skip_getopt = 0;

#if 0 && defined(PHP_DEBUG)
	/* IIS is always making things more difficult.  This allows
	 * us to stop PHP and attach a debugger before much gets started */
	{
		char szMessage [256];
		wsprintf (szMessage, "Please attach a debugger to the process 0x%X [%d] (%s) and click OK", GetCurrentProcessId(), GetCurrentProcessId(), argv[0]);
		MessageBox(NULL, szMessage, "CGI Debug Time!", MB_OK|MB_SERVICE_NOTIFICATION);
	}
#endif

#ifdef HAVE_SIGNAL_H
#if defined(SIGPIPE) && defined(SIG_IGN)
	signal(SIGPIPE, SIG_IGN); /* ignore SIGPIPE in standalone mode so
								that sockets created via fsockopen()
								don't kill PHP if the remote site
								closes it.  in apache|apxs mode apache
								does that for us!  thies@thieso.net
								20000419 */
#endif
#endif

#ifdef ZTS
	tsrm_startup(1, 1, 0, NULL);
	tsrm_ls = ts_resource(0);
#endif

	sapi_startup(&cgi_sapi_module);
	fastcgi = fcgi_is_fastcgi();
	cgi_sapi_module.php_ini_path_override = NULL;

#ifdef PHP_WIN32
	_fmode = _O_BINARY; /* sets default for file streams to binary */
	setmode(_fileno(stdin),  O_BINARY);	/* make the stdio mode be binary */
	setmode(_fileno(stdout), O_BINARY);	/* make the stdio mode be binary */
	setmode(_fileno(stderr), O_BINARY);	/* make the stdio mode be binary */
#endif

	if (!fastcgi) {
		/* Make sure we detect we are a cgi - a bit redundancy here,
		 * but the default case is that we have to check only the first one. */
		if (getenv("SERVER_SOFTWARE") ||
			getenv("SERVER_NAME") ||
			getenv("GATEWAY_INTERFACE") ||
			getenv("REQUEST_METHOD")
		) {
			cgi = 1;
		}
	}

	if((query_string = getenv("QUERY_STRING")) != NULL && strchr(query_string, '=') == NULL) {
		/* we've got query string that has no = - apache CGI will pass it to command line */
		unsigned char *p;
		decoded_query_string = strdup(query_string);
		php_url_decode(decoded_query_string, strlen(decoded_query_string));
		for (p = decoded_query_string; *p &&  *p <= ' '; p++) {
			/* skip all leading spaces */
		}
		if(*p == '-') {
			skip_getopt = 1;
		}
		free(decoded_query_string);
	}

	while (!skip_getopt && (c = php_getopt(argc, argv, OPTIONS, &php_optarg, &php_optind, 0, 2)) != -1) {
		switch (c) {
			case 'c':
				if (cgi_sapi_module.php_ini_path_override) {
					free(cgi_sapi_module.php_ini_path_override);
				}
				cgi_sapi_module.php_ini_path_override = strdup(php_optarg);
				break;
			case 'n':
				cgi_sapi_module.php_ini_ignore = 1;
				break;
			case 'd': {
				/* define ini entries on command line */
				int len = strlen(php_optarg);
				char *val;

				if ((val = strchr(php_optarg, '='))) {
					val++;
					if (!isalnum(*val) && *val != '"' && *val != '\'' && *val != '\0') {
						cgi_sapi_module.ini_entries = realloc(cgi_sapi_module.ini_entries, ini_entries_len + len + sizeof("\"\"\n\0"));
						memcpy(cgi_sapi_module.ini_entries + ini_entries_len, php_optarg, (val - php_optarg));
						ini_entries_len += (val - php_optarg);
						memcpy(cgi_sapi_module.ini_entries + ini_entries_len, "\"", 1);
						ini_entries_len++;
						memcpy(cgi_sapi_module.ini_entries + ini_entries_len, val, len - (val - php_optarg));
						ini_entries_len += len - (val - php_optarg);
						memcpy(cgi_sapi_module.ini_entries + ini_entries_len, "\"\n\0", sizeof("\"\n\0"));
						ini_entries_len += sizeof("\n\0\"") - 2;
					} else {
						cgi_sapi_module.ini_entries = realloc(cgi_sapi_module.ini_entries, ini_entries_len + len + sizeof("\n\0"));
						memcpy(cgi_sapi_module.ini_entries + ini_entries_len, php_optarg, len);
						memcpy(cgi_sapi_module.ini_entries + ini_entries_len + len, "\n\0", sizeof("\n\0"));
						ini_entries_len += len + sizeof("\n\0") - 2;
					}
				} else {
					cgi_sapi_module.ini_entries = realloc(cgi_sapi_module.ini_entries, ini_entries_len + len + sizeof("=1\n\0"));
					memcpy(cgi_sapi_module.ini_entries + ini_entries_len, php_optarg, len);
					memcpy(cgi_sapi_module.ini_entries + ini_entries_len + len, "=1\n\0", sizeof("=1\n\0"));
					ini_entries_len += len + sizeof("=1\n\0") - 2;
				}
				break;
			}
			/* if we're started on command line, check to see if
			 * we are being started as an 'external' fastcgi
			 * server by accepting a bindpath parameter. */
			case 'b':
				if (!fastcgi) {
					bindpath = strdup(php_optarg);
				}
				break;
			case 's': /* generate highlighted HTML from source */
				behavior = PHP_MODE_HIGHLIGHT;
				break;
		}
	}
	php_optind = orig_optind;
	php_optarg = orig_optarg;

	if (fastcgi || bindpath) {
		/* Override SAPI callbacks */
		cgi_sapi_module.ub_write     = sapi_fcgi_ub_write;
		cgi_sapi_module.flush        = sapi_fcgi_flush;
		cgi_sapi_module.read_post    = sapi_fcgi_read_post;
		cgi_sapi_module.getenv       = sapi_fcgi_getenv;
		cgi_sapi_module.read_cookies = sapi_fcgi_read_cookies;
	}

#ifdef ZTS
	SG(request_info).path_translated = NULL;
#endif

	cgi_sapi_module.executable_location = argv[0];
	if (!cgi && !fastcgi && !bindpath) {
		cgi_sapi_module.additional_functions = additional_functions;
	}

	/* startup after we get the above ini override se we get things right */
	if (cgi_sapi_module.startup(&cgi_sapi_module) == FAILURE) {
#ifdef ZTS
		tsrm_shutdown();
#endif
		return FAILURE;
	}

	/* check force_cgi after startup, so we have proper output */
	if (cgi && CGIG(force_redirect)) {
		/* Apache will generate REDIRECT_STATUS,
		 * Netscape and redirect.so will generate HTTP_REDIRECT_STATUS.
		 * redirect.so and installation instructions available from
		 * http://www.koehntopp.de/php.
		 *   -- kk@netuse.de
		 */
		if (!getenv("REDIRECT_STATUS") &&
			!getenv ("HTTP_REDIRECT_STATUS") &&
			/* this is to allow a different env var to be configured
			 * in case some server does something different than above */
			(!CGIG(redirect_status_env) || !getenv(CGIG(redirect_status_env)))
		) {
			zend_try {
				SG(sapi_headers).http_response_code = 400;
				PUTS("<b>Security Alert!</b> The PHP CGI cannot be accessed directly.\n\n\
<p>This PHP CGI binary was compiled with force-cgi-redirect enabled.  This\n\
means that a page will only be served up if the REDIRECT_STATUS CGI variable is\n\
set, e.g. via an Apache Action directive.</p>\n\
<p>For more information as to <i>why</i> this behaviour exists, see the <a href=\"http://php.net/security.cgi-bin\">\
manual page for CGI security</a>.</p>\n\
<p>For more information about changing this behaviour or re-enabling this webserver,\n\
consult the installation file that came with this distribution, or visit \n\
<a href=\"http://php.net/install.windows\">the manual page</a>.</p>\n");
			} zend_catch {
			} zend_end_try();
#if defined(ZTS) && !defined(PHP_DEBUG)
			/* XXX we're crashing here in msvc6 debug builds at
			 * php_message_handler_for_zend:839 because
			 * SG(request_info).path_translated is an invalid pointer.
			 * It still happens even though I set it to null, so something
			 * weird is going on.
			 */
			tsrm_shutdown();
#endif
			return FAILURE;
		}
	}

	if (bindpath) {
		fcgi_fd = fcgi_listen(bindpath, 128);
		if (fcgi_fd < 0) {
			fprintf(stderr, "Couldn't create FastCGI listen socket on port %s\n", bindpath);
#ifdef ZTS
			tsrm_shutdown();
#endif
			return FAILURE;
		}
		fastcgi = fcgi_is_fastcgi();
	}
	if (fastcgi) {
		/* How many times to run PHP scripts before dying */
		if (getenv("PHP_FCGI_MAX_REQUESTS")) {
			max_requests = atoi(getenv("PHP_FCGI_MAX_REQUESTS"));
			if (max_requests < 0) {
				fprintf(stderr, "PHP_FCGI_MAX_REQUESTS is not valid\n");
				return FAILURE;
			}
		}

		/* make php call us to get _ENV vars */
		php_php_import_environment_variables = php_import_environment_variables;
		php_import_environment_variables = cgi_php_import_environment_variables;

		/* library is already initialized, now init our request */
		request = fcgi_init_request(fcgi_fd);

#ifndef PHP_WIN32
	/* Pre-fork, if required */
	if (getenv("PHP_FCGI_CHILDREN")) {
		char * children_str = getenv("PHP_FCGI_CHILDREN");
		children = atoi(children_str);
		if (children < 0) {
			fprintf(stderr, "PHP_FCGI_CHILDREN is not valid\n");
			return FAILURE;
		}
		fcgi_set_mgmt_var("FCGI_MAX_CONNS", sizeof("FCGI_MAX_CONNS")-1, children_str, strlen(children_str));
		/* This is the number of concurrent requests, equals FCGI_MAX_CONNS */
		fcgi_set_mgmt_var("FCGI_MAX_REQS",  sizeof("FCGI_MAX_REQS")-1,  children_str, strlen(children_str));
	} else {
		fcgi_set_mgmt_var("FCGI_MAX_CONNS", sizeof("FCGI_MAX_CONNS")-1, "1", sizeof("1")-1);
		fcgi_set_mgmt_var("FCGI_MAX_REQS",  sizeof("FCGI_MAX_REQS")-1,  "1", sizeof("1")-1);
	}

	if (children) {
		int running = 0;
		pid_t pid;

		/* Create a process group for ourself & children */
		setsid();
		pgroup = getpgrp();
#ifdef DEBUG_FASTCGI
		fprintf(stderr, "Process group %d\n", pgroup);
#endif

		/* Set up handler to kill children upon exit */
		act.sa_flags = 0;
		act.sa_handler = fastcgi_cleanup;
		if (sigaction(SIGTERM, &act, &old_term) ||
			sigaction(SIGINT,  &act, &old_int)  ||
			sigaction(SIGQUIT, &act, &old_quit)
		) {
			perror("Can't set signals");
			exit(1);
		}

		if (fcgi_in_shutdown()) {
			goto parent_out;
		}

		while (parent) {
			do {
#ifdef DEBUG_FASTCGI
				fprintf(stderr, "Forking, %d running\n", running);
#endif
				pid = fork();
				switch (pid) {
				case 0:
					/* One of the children.
					 * Make sure we don't go round the
					 * fork loop any more
					 */
					parent = 0;

					/* don't catch our signals */
					sigaction(SIGTERM, &old_term, 0);
					sigaction(SIGQUIT, &old_quit, 0);
					sigaction(SIGINT,  &old_int,  0);
					break;
				case -1:
					perror("php (pre-forking)");
					exit(1);
					break;
				default:
					/* Fine */
					running++;
					break;
				}
			} while (parent && (running < children));

			if (parent) {
#ifdef DEBUG_FASTCGI
				fprintf(stderr, "Wait for kids, pid %d\n", getpid());
#endif
				parent_waiting = 1;
				while (1) {
					if (wait(&status) >= 0) {
						running--;
						break;
					} else if (exit_signal) {
						break;
					}
				}
				if (exit_signal) {
#if 0
					while (running > 0) {
						while (wait(&status) < 0) {
						}
						running--;
					}
#endif
					goto parent_out;
				}
			}
		}
	} else {
		parent = 0;
	}

#endif /* WIN32 */
	}

	zend_first_try {
		while (!skip_getopt && (c = php_getopt(argc, argv, OPTIONS, &php_optarg, &php_optind, 1, 2)) != -1) {
			switch (c) {
				case 'T':
					benchmark = 1;
					repeats = atoi(php_optarg);
#ifdef HAVE_GETTIMEOFDAY
					gettimeofday(&start, NULL);
#else
					time(&start);
#endif
					break;
				case 'h':
				case '?':
					if (request) {
						fcgi_destroy_request(request);
					}
					fcgi_shutdown();
					no_headers = 1;
					SG(headers_sent) = 1;
					php_cgi_usage(argv[0]);
					php_output_end_all(TSRMLS_C);
					exit_status = 0;
					goto out;
			}
		}
		php_optind = orig_optind;
		php_optarg = orig_optarg;

		/* start of FAST CGI loop */
		/* Initialise FastCGI request structure */
#ifdef PHP_WIN32
		/* attempt to set security impersonation for fastcgi
		 * will only happen on NT based OS, others will ignore it. */
		if (fastcgi && CGIG(impersonate)) {
			fcgi_impersonate();
		}
#endif
		while (!fastcgi || fcgi_accept_request(request) >= 0) {
			SG(server_context) = fastcgi ? (void *) request : (void *) 1;
			init_request_info(request TSRMLS_CC);
			CG(interactive) = 0;

			if (!cgi && !fastcgi) {
				while ((c = php_getopt(argc, argv, OPTIONS, &php_optarg, &php_optind, 0, 2)) != -1) {
					switch (c) {

						case 'a':	/* interactive mode */
							printf("Interactive mode enabled\n\n");
							CG(interactive) = 1;
							break;

						case 'C': /* don't chdir to the script directory */
							SG(options) |= SAPI_OPTION_NO_CHDIR;
							break;

						case 'e': /* enable extended info output */
							CG(compiler_options) |= ZEND_COMPILE_EXTENDED_INFO;
							break;

						case 'f': /* parse file */
							if (script_file) {
								efree(script_file);
							}
							script_file = estrdup(php_optarg);
							no_headers = 1;
							break;

						case 'i': /* php info & quit */
							if (script_file) {
								efree(script_file);
							}
							if (php_request_startup(TSRMLS_C) == FAILURE) {
								SG(server_context) = NULL;
								php_module_shutdown(TSRMLS_C);
								return FAILURE;
							}
							if (no_headers) {
								SG(headers_sent) = 1;
								SG(request_info).no_headers = 1;
							}
							php_print_info(0xFFFFFFFF TSRMLS_CC);
							php_request_shutdown((void *) 0);
							fcgi_shutdown();
							exit_status = 0;
							goto out;

						case 'l': /* syntax check mode */
							no_headers = 1;
							behavior = PHP_MODE_LINT;
							break;

						case 'm': /* list compiled in modules */
							if (script_file) {
								efree(script_file);
							}
							SG(headers_sent) = 1;
							php_printf("[PHP Modules]\n");
							print_modules(TSRMLS_C);
							php_printf("\n[Zend Modules]\n");
							print_extensions(TSRMLS_C);
							php_printf("\n");
							php_output_end_all(TSRMLS_C);
							fcgi_shutdown();
							exit_status = 0;
							goto out;

#if 0 /* not yet operational, see also below ... */
						case '': /* generate indented source mode*/
							behavior=PHP_MODE_INDENT;
							break;
#endif

						case 'q': /* do not generate HTTP headers */
							no_headers = 1;
							break;

						case 'v': /* show php version & quit */
							if (script_file) {
								efree(script_file);
							}
							no_headers = 1;
							if (php_request_startup(TSRMLS_C) == FAILURE) {
								SG(server_context) = NULL;
								php_module_shutdown(TSRMLS_C);
								return FAILURE;
							}
							if (no_headers) {
								SG(headers_sent) = 1;
								SG(request_info).no_headers = 1;
							}
#if ZEND_DEBUG
							php_printf("PHP %s (%s) (built: %s %s) (DEBUG)\nCopyright (c) 1997-2014 The PHP Group\n%s", PHP_VERSION, sapi_module.name, __DATE__, __TIME__, get_zend_version());
#else
							php_printf("PHP %s (%s) (built: %s %s)\nCopyright (c) 1997-2014 The PHP Group\n%s", PHP_VERSION, sapi_module.name, __DATE__, __TIME__, get_zend_version());
#endif
							php_request_shutdown((void *) 0);
							fcgi_shutdown();
							exit_status = 0;
							goto out;

						case 'w':
							behavior = PHP_MODE_STRIP;
							break;

						case 'z': /* load extension file */
							zend_load_extension(php_optarg);
							break;

						default:
							break;
					}
				}

				if (script_file) {
					/* override path_translated if -f on command line */
					STR_FREE(SG(request_info).path_translated);
					SG(request_info).path_translated = script_file;
					/* before registering argv to module exchange the *new* argv[0] */
					/* we can achieve this without allocating more memory */
					SG(request_info).argc = argc - (php_optind - 1);
					SG(request_info).argv = &argv[php_optind - 1];
					SG(request_info).argv[0] = script_file;
				} else if (argc > php_optind) {
					/* file is on command line, but not in -f opt */
					STR_FREE(SG(request_info).path_translated);
					SG(request_info).path_translated = estrdup(argv[php_optind]);
					/* arguments after the file are considered script args */
					SG(request_info).argc = argc - php_optind;
					SG(request_info).argv = &argv[php_optind];
				}

				if (no_headers) {
					SG(headers_sent) = 1;
					SG(request_info).no_headers = 1;
				}

				/* all remaining arguments are part of the query string
				 * this section of code concatenates all remaining arguments
				 * into a single string, seperating args with a &
				 * this allows command lines like:
				 *
				 *  test.php v1=test v2=hello+world!
				 *  test.php "v1=test&v2=hello world!"
				 *  test.php v1=test "v2=hello world!"
				*/
				if (!SG(request_info).query_string && argc > php_optind) {
					int slen = strlen(PG(arg_separator).input);
					len = 0;
					for (i = php_optind; i < argc; i++) {
						if (i < (argc - 1)) {
							len += strlen(argv[i]) + slen;
						} else {
							len += strlen(argv[i]);
						}
					}

					len += 2;
					s = malloc(len);
					*s = '\0';			/* we are pretending it came from the environment  */
					for (i = php_optind; i < argc; i++) {
						strlcat(s, argv[i], len);
						if (i < (argc - 1)) {
							strlcat(s, PG(arg_separator).input, len);
						}
					}
					SG(request_info).query_string = s;
					free_query_string = 1;
				}
			} /* end !cgi && !fastcgi */

			/*
				we never take stdin if we're (f)cgi, always
				rely on the web server giving us the info
				we need in the environment.
			*/
			if (SG(request_info).path_translated || cgi || fastcgi) {
				file_handle.type = ZEND_HANDLE_FILENAME;
				file_handle.filename = SG(request_info).path_translated;
				file_handle.handle.fp = NULL;
			} else {
				file_handle.filename = "-";
				file_handle.type = ZEND_HANDLE_FP;
				file_handle.handle.fp = stdin;
			}

			file_handle.opened_path = NULL;
			file_handle.free_filename = 0;

			/* request startup only after we've done all we can to
			 * get path_translated */
			if (php_request_startup(TSRMLS_C) == FAILURE) {
				if (fastcgi) {
					fcgi_finish_request(request, 1);
				}
				SG(server_context) = NULL;
				php_module_shutdown(TSRMLS_C);
				return FAILURE;
			}
			if (no_headers) {
				SG(headers_sent) = 1;
				SG(request_info).no_headers = 1;
			}

			/*
				at this point path_translated will be set if:
				1. we are running from shell and got filename was there
				2. we are running as cgi or fastcgi
			*/
			if (cgi || fastcgi || SG(request_info).path_translated) {
				if (php_fopen_primary_script(&file_handle TSRMLS_CC) == FAILURE) {
					zend_try {
						if (errno == EACCES) {
							SG(sapi_headers).http_response_code = 403;
							PUTS("Access denied.\n");
						} else {
							SG(sapi_headers).http_response_code = 404;
							PUTS("No input file specified.\n");
						}
					} zend_catch {
					} zend_end_try();
					/* we want to serve more requests if this is fastcgi
					 * so cleanup and continue, request shutdown is
					 * handled later */
					if (fastcgi) {
						goto fastcgi_request_done;
					}

					STR_FREE(SG(request_info).path_translated);

					if (free_query_string && SG(request_info).query_string) {
						free(SG(request_info).query_string);
						SG(request_info).query_string = NULL;
					}

					php_request_shutdown((void *) 0);
					SG(server_context) = NULL;
					php_module_shutdown(TSRMLS_C);
					sapi_shutdown();
#ifdef ZTS
					tsrm_shutdown();
#endif
					return FAILURE;
				}
			}

			if (CGIG(check_shebang_line)) {
				/* #!php support */
				switch (file_handle.type) {
					case ZEND_HANDLE_FD:
						if (file_handle.handle.fd < 0) {
							break;
						}
						file_handle.type = ZEND_HANDLE_FP;
						file_handle.handle.fp = fdopen(file_handle.handle.fd, "rb");
						/* break missing intentionally */
					case ZEND_HANDLE_FP:
						if (!file_handle.handle.fp ||
						    (file_handle.handle.fp == stdin)) {
							break;
						}
						c = fgetc(file_handle.handle.fp);
						if (c == '#') {
							while (c != '\n' && c != '\r' && c != EOF) {
								c = fgetc(file_handle.handle.fp);	/* skip to end of line */
							}
							/* handle situations where line is terminated by \r\n */
							if (c == '\r') {
								if (fgetc(file_handle.handle.fp) != '\n') {
									long pos = ftell(file_handle.handle.fp);
									fseek(file_handle.handle.fp, pos - 1, SEEK_SET);
								}
							}
							CG(start_lineno) = 2;
						} else {
							rewind(file_handle.handle.fp);
						}
						break;
					case ZEND_HANDLE_STREAM:
						c = php_stream_getc((php_stream*)file_handle.handle.stream.handle);
						if (c == '#') {
							while (c != '\n' && c != '\r' && c != EOF) {
								c = php_stream_getc((php_stream*)file_handle.handle.stream.handle);	/* skip to end of line */
							}
							/* handle situations where line is terminated by \r\n */
							if (c == '\r') {
								if (php_stream_getc((php_stream*)file_handle.handle.stream.handle) != '\n') {
									long pos = php_stream_tell((php_stream*)file_handle.handle.stream.handle);
									php_stream_seek((php_stream*)file_handle.handle.stream.handle, pos - 1, SEEK_SET);
								}
							}
							CG(start_lineno) = 2;
						} else {
							php_stream_rewind((php_stream*)file_handle.handle.stream.handle);
						}
						break;
					case ZEND_HANDLE_MAPPED:
						if (file_handle.handle.stream.mmap.buf[0] == '#') {
                                                    int i = 1;
 
                                                    c = file_handle.handle.stream.mmap.buf[i++];
                                                       while (c != '\n' && c != '\r' && i < file_handle.handle.stream.mmap.len) {
                                                                c = file_handle.handle.stream.mmap.buf[i++];
                                                        }
                                                        if (c == '\r') {
                                                               if (i < file_handle.handle.stream.mmap.len && file_handle.handle.stream.mmap.buf[i] == '\n') {
                                                                        i++;
                                                                }
                                                        }
                                                       if(i > file_handle.handle.stream.mmap.len) {
                                                               i = file_handle.handle.stream.mmap.len;
                                                       }
                                                        file_handle.handle.stream.mmap.buf += i;
                                                        file_handle.handle.stream.mmap.len -= i;
                                                }
				}
			}

			switch (behavior) {
				case PHP_MODE_STANDARD:
					php_execute_script(&file_handle TSRMLS_CC);
					break;
				case PHP_MODE_LINT:
					PG(during_request_startup) = 0;
					exit_status = php_lint_script(&file_handle TSRMLS_CC);
					if (exit_status == SUCCESS) {
						zend_printf("No syntax errors detected in %s\n", file_handle.filename);
					} else {
						zend_printf("Errors parsing %s\n", file_handle.filename);
					}
					break;
				case PHP_MODE_STRIP:
					if (open_file_for_scanning(&file_handle TSRMLS_CC) == SUCCESS) {
						zend_strip(TSRMLS_C);
						zend_file_handle_dtor(&file_handle TSRMLS_CC);
						php_output_teardown();
					}
					return SUCCESS;
					break;
				case PHP_MODE_HIGHLIGHT:
					{
						zend_syntax_highlighter_ini syntax_highlighter_ini;

						if (open_file_for_scanning(&file_handle TSRMLS_CC) == SUCCESS) {
							php_get_highlight_struct(&syntax_highlighter_ini);
							zend_highlight(&syntax_highlighter_ini TSRMLS_CC);
							if (fastcgi) {
								goto fastcgi_request_done;
							}
							zend_file_handle_dtor(&file_handle TSRMLS_CC);
							php_output_teardown();
						}
						return SUCCESS;
					}
					break;
#if 0
				/* Zeev might want to do something with this one day */
				case PHP_MODE_INDENT:
					open_file_for_scanning(&file_handle TSRMLS_CC);
					zend_indent();
					zend_file_handle_dtor(&file_handle TSRMLS_CC);
					php_output_teardown();
					return SUCCESS;
					break;
#endif
			}

fastcgi_request_done:
			{
				STR_FREE(SG(request_info).path_translated);

				php_request_shutdown((void *) 0);

				if (exit_status == 0) {
					exit_status = EG(exit_status);
				}

				if (free_query_string && SG(request_info).query_string) {
					free(SG(request_info).query_string);
					SG(request_info).query_string = NULL;
				}
			}

			if (!fastcgi) {
				if (benchmark) {
					repeats--;
					if (repeats > 0) {
						script_file = NULL;
						php_optind = orig_optind;
						php_optarg = orig_optarg;
						continue;
					}
				}
				break;
			}

			/* only fastcgi will get here */
			requests++;
			if (max_requests && (requests == max_requests)) {
				fcgi_finish_request(request, 1);
				if (bindpath) {
					free(bindpath);
				}
				if (max_requests != 1) {
					/* no need to return exit_status of the last request */
					exit_status = 0;
				}
				break;
			}
			/* end of fastcgi loop */
		}
		if (request) {
			fcgi_destroy_request(request);
		}
		fcgi_shutdown();

		if (cgi_sapi_module.php_ini_path_override) {
			free(cgi_sapi_module.php_ini_path_override);
		}
		if (cgi_sapi_module.ini_entries) {
			free(cgi_sapi_module.ini_entries);
		}
	} zend_catch {
		exit_status = 255;
	} zend_end_try();

out:
	if (benchmark) {
		int sec;
#ifdef HAVE_GETTIMEOFDAY
		int usec;

		gettimeofday(&end, NULL);
		sec = (int)(end.tv_sec - start.tv_sec);
		if (end.tv_usec >= start.tv_usec) {
			usec = (int)(end.tv_usec - start.tv_usec);
		} else {
			sec -= 1;
			usec = (int)(end.tv_usec + 1000000 - start.tv_usec);
		}
		fprintf(stderr, "\nElapsed time: %d.%06d sec\n", sec, usec);
#else
		time(&end);
		sec = (int)(end - start);
		fprintf(stderr, "\nElapsed time: %d sec\n", sec);
#endif
	}

#ifndef PHP_WIN32
parent_out:
#endif

	SG(server_context) = NULL;
	php_module_shutdown(TSRMLS_C);
	sapi_shutdown();

#ifdef ZTS
	tsrm_shutdown();
#endif

#if defined(PHP_WIN32) && ZEND_DEBUG && 0
	_CrtDumpMemoryLeaks();
#endif

	return exit_status;
}
