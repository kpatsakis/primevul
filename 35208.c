int do_run(gchar* command, gchar* file) {
	gchar* cmd;
	int retval=0;

	if(command && *command) {
		cmd = g_strdup_printf(command, file);
		retval=system(cmd);
		g_free(cmd);
	}
	return retval;
}
