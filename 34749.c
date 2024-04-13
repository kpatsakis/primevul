static int FAST_FUNC config_file_action(const char *filename,
					struct stat *statbuf UNUSED_PARAM,
					void *userdata UNUSED_PARAM,
					int depth UNUSED_PARAM)
{
	char *tokens[3];
	parser_t *p;
	struct module_entry *m;
	int rc = TRUE;

	if (bb_basename(filename)[0] == '.')
		goto error;

	p = config_open2(filename, fopen_for_read);
	if (p == NULL) {
		rc = FALSE;
		goto error;
	}

	while (config_read(p, tokens, 3, 2, "# \t", PARSE_NORMAL)) {
		if (strcmp(tokens[0], "alias") == 0) {
			/* alias <wildcard> <modulename> */
			llist_t *l;
			char wildcard[MODULE_NAME_LEN];
			char *rmod;

			if (tokens[2] == NULL)
				continue;
			filename2modname(tokens[1], wildcard);

			for (l = G.probes; l; l = l->link) {
				m = (struct module_entry *) l->data;
				if (fnmatch(wildcard, m->modname, 0) != 0)
					continue;
				rmod = filename2modname(tokens[2], NULL);
				llist_add_to(&m->realnames, rmod);

				if (m->flags & MODULE_FLAG_NEED_DEPS) {
					m->flags &= ~MODULE_FLAG_NEED_DEPS;
					G.num_unresolved_deps--;
				}

				m = get_or_add_modentry(rmod);
				if (!(m->flags & MODULE_FLAG_NEED_DEPS)) {
					m->flags |= MODULE_FLAG_NEED_DEPS;
					G.num_unresolved_deps++;
				}
			}
		} else if (strcmp(tokens[0], "options") == 0) {
			/* options <modulename> <option...> */
			if (tokens[2] == NULL)
				continue;
			m = get_or_add_modentry(tokens[1]);
			m->options = gather_options_str(m->options, tokens[2]);
		} else if (strcmp(tokens[0], "include") == 0) {
			/* include <filename> */
			read_config(tokens[1]);
		} else if (ENABLE_FEATURE_MODPROBE_BLACKLIST
		 && strcmp(tokens[0], "blacklist") == 0
		) {
			/* blacklist <modulename> */
			get_or_add_modentry(tokens[1])->flags |= MODULE_FLAG_BLACKLISTED;
		}
	}
	config_close(p);
 error:
	return rc;
}
