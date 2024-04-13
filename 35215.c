int main(int argc, char *argv[]) {
	SERVER *serve;
	GArray *servers;
	GError *err=NULL;

	if (sizeof( struct nbd_request )!=28) {
		fprintf(stderr,"Bad size of structure. Alignment problems?\n");
		exit(EXIT_FAILURE) ;
	}

	memset(pidftemplate, '\0', 256);

	logging();
	config_file_pos = g_strdup(CFILE);
	serve=cmdline(argc, argv);
	servers = parse_cfile(config_file_pos, &err);
	
	if(serve) {
		serve->socket_family = AF_UNSPEC;

		append_serve(serve, servers);
     
		if (!(serve->port)) {
			CLIENT *client;
#ifndef ISSERVER
			/* You really should define ISSERVER if you're going to use
			 * inetd mode, but if you don't, closing stdout and stderr
			 * (which inetd had connected to the client socket) will let it
			 * work. */
			close(1);
			close(2);
			open("/dev/null", O_WRONLY);
			open("/dev/null", O_WRONLY);
			g_log_set_default_handler( glib_message_syslog_redirect, NULL );
#endif
			client=g_malloc(sizeof(CLIENT));
			client->server=serve;
			client->net=0;
			client->exportsize=OFFT_MAX;
			set_peername(0,client);
			serveconnection(client);
			return 0;
		}
	}
    
	if(!servers || !servers->len) {
		if(err && !(err->domain == g_quark_from_string("parse_cfile")
				&& err->code == CFILE_NOTFOUND)) {
			g_warning("Could not parse config file: %s", 
					err ? err->message : "Unknown error");
		}
	}
	if(serve) {
		g_warning("Specifying an export on the command line is deprecated.");
		g_warning("Please use a configuration file instead.");
	}

	if((!serve) && (!servers||!servers->len)) {
		g_message("No configured exports; quitting.");
		exit(EXIT_FAILURE);
	}
	daemonize(serve);
	setup_servers(servers);
	dousers();
	serveloop(servers);
	return 0 ;
}
