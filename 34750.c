static int do_modprobe(struct module_entry *m)
{
	int rc, first;

	if (!(m->flags & MODULE_FLAG_FOUND_IN_MODDEP)) {
		if (!(option_mask32 & INSMOD_OPT_SILENT))
			bb_error_msg("module %s not found in modules.dep",
				humanly_readable_name(m));
		return -ENOENT;
	}
	DBG("do_modprob'ing %s", m->modname);

	if (!(option_mask32 & OPT_REMOVE))
		m->deps = llist_rev(m->deps);

	if (0) {
		llist_t *l;
		for (l = m->deps; l; l = l->link)
			DBG("dep: %s", l->data);
	}

	first = 1;
	rc = 0;
	while (m->deps) {
		struct module_entry *m2;
		char *fn, *options;

		rc = 0;
		fn = llist_pop(&m->deps); /* we leak it */
		m2 = get_or_add_modentry(fn);

		if (option_mask32 & OPT_REMOVE) {
			/* modprobe -r */
			if (m2->flags & MODULE_FLAG_LOADED) {
				rc = bb_delete_module(m2->modname, O_EXCL);
				if (rc) {
					if (first) {
						bb_error_msg("can't unload module %s: %s",
							humanly_readable_name(m2),
							moderror(rc));
						break;
					}
				} else {
					m2->flags &= ~MODULE_FLAG_LOADED;
				}
			}
			/* do not error out if *deps* fail to unload */
			first = 0;
			continue;
		}

		options = m2->options;
		m2->options = NULL;
		options = parse_and_add_kcmdline_module_options(options, m2->modname);
		if (m == m2)
			options = gather_options_str(options, G.cmdline_mopts);

		if (option_mask32 & OPT_SHOW_DEPS) {
			printf(options ? "insmod %s/%s/%s %s\n"
					: "insmod %s/%s/%s\n",
				CONFIG_DEFAULT_MODULES_DIR, G.uts.release, fn,
				options);
			free(options);
			continue;
		}

		if (m2->flags & MODULE_FLAG_LOADED) {
			DBG("%s is already loaded, skipping", fn);
			free(options);
			continue;
		}

		rc = bb_init_module(fn, options);
		DBG("loaded %s '%s', rc:%d", fn, options, rc);
		if (rc == EEXIST)
			rc = 0;
		free(options);
		if (rc) {
			bb_error_msg("can't load module %s (%s): %s",
				humanly_readable_name(m2),
				fn,
				moderror(rc)
			);
			break;
		}
		m2->flags |= MODULE_FLAG_LOADED;
	}

	return rc;
}
