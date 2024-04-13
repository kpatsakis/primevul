SERVER* cmdline(int argc, char *argv[]) {
	int i=0;
	int nonspecial=0;
	int c;
	struct option long_options[] = {
		{"read-only", no_argument, NULL, 'r'},
		{"multi-file", no_argument, NULL, 'm'},
		{"copy-on-write", no_argument, NULL, 'c'},
		{"authorize-file", required_argument, NULL, 'l'},
		{"config-file", required_argument, NULL, 'C'},
		{"pid-file", required_argument, NULL, 'p'},
		{"output-config", required_argument, NULL, 'o'},
		{"max-connection", required_argument, NULL, 'M'},
		{0,0,0,0}
	};
	SERVER *serve;
	off_t es;
	size_t last;
	char suffix;
	gboolean do_output=FALSE;
	gchar* section_header="";
	gchar** addr_port;

	if(argc==1) {
		return NULL;
	}
	serve=g_new0(SERVER, 1);
	serve->authname = g_strdup(default_authname);
	serve->virtstyle=VIRT_IPLIT;
	while((c=getopt_long(argc, argv, "-C:cl:mo:rp:M:", long_options, &i))>=0) {
		switch (c) {
		case 1:
			/* non-option argument */
			switch(nonspecial++) {
			case 0:
				if(strchr(optarg, ':') == strrchr(optarg, ':')) {
					addr_port=g_strsplit(optarg, ":", 2);

					/* Check for "@" - maybe user using this separator
						 for IPv4 address */
					if(!addr_port[1]) {
						g_strfreev(addr_port);
						addr_port=g_strsplit(optarg, "@", 2);
					}
				} else {
					addr_port=g_strsplit(optarg, "@", 2);
				}

				if(addr_port[1]) {
					serve->port=strtol(addr_port[1], NULL, 0);
					serve->listenaddr=g_strdup(addr_port[0]);
				} else {
					serve->listenaddr=NULL;
					serve->port=strtol(addr_port[0], NULL, 0);
				}
				g_strfreev(addr_port);
				break;
			case 1:
				serve->exportname = g_strdup(optarg);
				if(serve->exportname[0] != '/') {
					fprintf(stderr, "E: The to be exported file needs to be an absolute filename!\n");
					exit(EXIT_FAILURE);
				}
				break;
			case 2:
				last=strlen(optarg)-1;
				suffix=optarg[last];
				if (suffix == 'k' || suffix == 'K' ||
				    suffix == 'm' || suffix == 'M')
					optarg[last] = '\0';
				es = (off_t)atoll(optarg);
				switch (suffix) {
					case 'm':
					case 'M':  es <<= 10;
					case 'k':
					case 'K':  es <<= 10;
					default :  break;
				}
				serve->expected_size = es;
				break;
			}
			break;
		case 'r':
			serve->flags |= F_READONLY;
			break;
		case 'm':
			serve->flags |= F_MULTIFILE;
			break;
		case 'o':
			do_output = TRUE;
			section_header = g_strdup(optarg);
			break;
		case 'p':
			strncpy(pidftemplate, optarg, 256);
			break;
		case 'c': 
			serve->flags |=F_COPYONWRITE;
		        break;
		case 'C':
			g_free(config_file_pos);
			config_file_pos=g_strdup(optarg);
			break;
		case 'l':
			g_free(serve->authname);
			serve->authname=g_strdup(optarg);
			break;
		case 'M':
			serve->max_connections = strtol(optarg, NULL, 0);
			break;
		default:
			usage();
			exit(EXIT_FAILURE);
			break;
		}
	}
	/* What's left: the port to export, the name of the to be exported
	 * file, and, optionally, the size of the file, in that order. */
	if(nonspecial<2) {
		g_free(serve);
		serve=NULL;
	} else {
		do_oldstyle = TRUE;
	}
	if(do_output) {
		if(!serve) {
			g_critical("Need a complete configuration on the command line to output a config file section!");
			exit(EXIT_FAILURE);
		}
		dump_section(serve, section_header);
	}
	return serve;
}
