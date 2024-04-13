static char *__func_name_from_ord(const char *module, ut16 ordinal) {
	if (!module) {
		return NULL;
	}
	char *lower_module = strdup (module);
	r_str_case (lower_module, false);
	char *path = r_str_newf (R_JOIN_4_PATHS ("%s", R2_SDB_FORMAT, "dll", "%s.sdb"), r_sys_prefix (NULL), lower_module);
	free (lower_module);
	char *ord = r_str_newf ("%d", ordinal);
	char *name;
	if (r_file_exists (path)) {
		Sdb *sdb = sdb_new (NULL, path, 0);
		name = sdb_get (sdb, ord, NULL);
		if (!name) {
			name = ord;
		} else {
			free (ord);
		}
		sdb_close (sdb);
		free (sdb);
	} else {
		name = ord;
	}
	free (path);
	return name;
}