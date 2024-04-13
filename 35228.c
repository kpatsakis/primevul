void setup_servers(GArray* servers) {
	int i;
	struct sigaction sa;
	int want_modern=0;

	for(i=0;i<servers->len;i++) {
		want_modern |= setup_serve(&(g_array_index(servers, SERVER, i)));
	}
	if(want_modern) {
		open_modern();
	}
	children=g_hash_table_new_full(g_int_hash, g_int_equal, NULL, destroy_pid_t);

	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if(sigaction(SIGCHLD, &sa, NULL) == -1)
		err("sigaction: %m");
	sa.sa_handler = sigterm_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if(sigaction(SIGTERM, &sa, NULL) == -1)
		err("sigaction: %m");
}
