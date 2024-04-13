static void udhcp_run_script(struct dhcp_packet *packet, const char *name)
{
	char **envp, **curr;
	char *argv[3];

	envp = fill_envp(packet);

	/* call script */
	log1("Executing %s %s", client_config.script, name);
	argv[0] = (char*) client_config.script;
	argv[1] = (char*) name;
	argv[2] = NULL;
	spawn_and_wait(argv);

	for (curr = envp; *curr; curr++) {
		log2(" %s", *curr);
		bb_unsetenv_and_free(*curr);
	}
	free(envp);
}
