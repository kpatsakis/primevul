CWD_API int virtual_file_ex(cwd_state *state, const char *path, verify_path_func verify_path, int use_realpath TSRMLS_DC) /* {{{ */
{
	int path_length = strlen(path);
	char resolved_path[MAXPATHLEN];
	int start = 1;
	int ll = 0;
	time_t t;
	int ret;
	int add_slash;
	void *tmp;

	if (path_length == 0 || path_length >= MAXPATHLEN-1) {
#ifdef TSRM_WIN32
# if _MSC_VER < 1300
		errno = EINVAL;
# else
		_set_errno(EINVAL);
# endif
#else
		errno = EINVAL;
#endif
		return 1;
	}

#if VIRTUAL_CWD_DEBUG
	fprintf(stderr,"cwd = %s path = %s\n", state->cwd, path);
#endif

	/* cwd_length can be 0 when getcwd() fails.
	 * This can happen under solaris when a dir does not have read permissions
	 * but *does* have execute permissions */
	if (!IS_ABSOLUTE_PATH(path, path_length)) {
		if (state->cwd_length == 0) {
			/* resolve relative path */
			start = 0;
			memcpy(resolved_path , path, path_length + 1);
		} else {
			int state_cwd_length = state->cwd_length;

#ifdef TSRM_WIN32
			if (IS_SLASH(path[0])) {
				if (state->cwd[1] == ':') {
					/* Copy only the drive name */
					state_cwd_length = 2;
				} else if (IS_UNC_PATH(state->cwd, state->cwd_length)) {
					/* Copy only the share name */
					state_cwd_length = 2;
					while (IS_SLASH(state->cwd[state_cwd_length])) {
						state_cwd_length++;
					}
					while (state->cwd[state_cwd_length] &&
							!IS_SLASH(state->cwd[state_cwd_length])) {
						state_cwd_length++;
					}
					while (IS_SLASH(state->cwd[state_cwd_length])) {
						state_cwd_length++;
					}
					while (state->cwd[state_cwd_length] &&
							!IS_SLASH(state->cwd[state_cwd_length])) {
						state_cwd_length++;
					}
				}
			}
#endif
			if (path_length + state_cwd_length + 1 >= MAXPATHLEN-1) {
				return 1;
			}
			memcpy(resolved_path, state->cwd, state_cwd_length);
			if (resolved_path[state_cwd_length-1] == DEFAULT_SLASH) {
				memcpy(resolved_path + state_cwd_length, path, path_length + 1);
				path_length += state_cwd_length;
			} else {
				resolved_path[state_cwd_length] = DEFAULT_SLASH;
				memcpy(resolved_path + state_cwd_length + 1, path, path_length + 1);
				path_length += state_cwd_length + 1;
			}
		}
	} else {
#ifdef TSRM_WIN32
		if (path_length > 2 && path[1] == ':' && !IS_SLASH(path[2])) {
			resolved_path[0] = path[0];
			resolved_path[1] = ':';
			resolved_path[2] = DEFAULT_SLASH;
			memcpy(resolved_path + 3, path + 2, path_length - 1);
			path_length++;
		} else
#endif
		memcpy(resolved_path, path, path_length + 1);
	}

#ifdef TSRM_WIN32
	if (memchr(resolved_path, '*', path_length) ||
		memchr(resolved_path, '?', path_length)) {
		return 1;
	}
#endif

#ifdef TSRM_WIN32
	if (IS_UNC_PATH(resolved_path, path_length)) {
		/* skip UNC name */
		resolved_path[0] = DEFAULT_SLASH;
		resolved_path[1] = DEFAULT_SLASH;
		start = 2;
		while (!IS_SLASH(resolved_path[start])) {
			if (resolved_path[start] == 0) {
				goto verify;
			}
			resolved_path[start] = toupper(resolved_path[start]);
			start++;
		}
		resolved_path[start++] = DEFAULT_SLASH;
		while (!IS_SLASH(resolved_path[start])) {
			if (resolved_path[start] == 0) {
				goto verify;
			}
			resolved_path[start] = toupper(resolved_path[start]);
			start++;
		}
		resolved_path[start++] = DEFAULT_SLASH;
	} else if (IS_ABSOLUTE_PATH(resolved_path, path_length)) {
		/* skip DRIVE name */
		resolved_path[0] = toupper(resolved_path[0]);
		resolved_path[2] = DEFAULT_SLASH;
		start = 3;
	}
#elif defined(NETWARE)
	if (IS_ABSOLUTE_PATH(resolved_path, path_length)) {
		/* skip VOLUME name */
		start = 0;
		while (start != ':') {
			if (resolved_path[start] == 0) return -1;
			start++;
		}
		start++;
		if (!IS_SLASH(resolved_path[start])) return -1;
		resolved_path[start++] = DEFAULT_SLASH;
	}
#endif

	add_slash = (use_realpath != CWD_REALPATH) && path_length > 0 && IS_SLASH(resolved_path[path_length-1]);
	t = CWDG(realpath_cache_ttl) ? 0 : -1;
	path_length = tsrm_realpath_r(resolved_path, start, path_length, &ll, &t, use_realpath, 0, NULL TSRMLS_CC);

	if (path_length < 0) {
		errno = ENOENT;
		return 1;
	}

	if (!start && !path_length) {
		resolved_path[path_length++] = '.';
	}
	if (add_slash && path_length && !IS_SLASH(resolved_path[path_length-1])) {
		if (path_length >= MAXPATHLEN-1) {
			return -1;
		}
		resolved_path[path_length++] = DEFAULT_SLASH;
	}
	resolved_path[path_length] = 0;

#ifdef TSRM_WIN32
verify:
#endif
	if (verify_path) {
		cwd_state old_state;

		CWD_STATE_COPY(&old_state, state);
		state->cwd_length = path_length;

		tmp = realloc(state->cwd, state->cwd_length+1);
		if (tmp == NULL) {
#if VIRTUAL_CWD_DEBUG
			fprintf (stderr, "Out of memory\n");
#endif
			return 1;
		}
		state->cwd = (char *) tmp;

		memcpy(state->cwd, resolved_path, state->cwd_length+1);
		if (verify_path(state)) {
			CWD_STATE_FREE(state);
			*state = old_state;
			ret = 1;
		} else {
			CWD_STATE_FREE(&old_state);
			ret = 0;
		}
	} else {
		state->cwd_length = path_length;
		tmp = realloc(state->cwd, state->cwd_length+1);
		if (tmp == NULL) {
#if VIRTUAL_CWD_DEBUG
			fprintf (stderr, "Out of memory\n");
#endif
			return 1;
		}
		state->cwd = (char *) tmp;

		memcpy(state->cwd, resolved_path, state->cwd_length+1);
		ret = 0;
	}

#if VIRTUAL_CWD_DEBUG
	fprintf (stderr, "virtual_file_ex() = %s\n",state->cwd);
#endif
	return (ret);
}