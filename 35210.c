void dump_section(SERVER* serve, gchar* section_header) {
	printf("[%s]\n", section_header);
	printf("\texportname = %s\n", serve->exportname);
	printf("\tlistenaddr = %s\n", serve->listenaddr);
	printf("\tport = %d\n", serve->port);
	if(serve->flags & F_READONLY) {
		printf("\treadonly = true\n");
	}
	if(serve->flags & F_MULTIFILE) {
		printf("\tmultifile = true\n");
	}
	if(serve->flags & F_COPYONWRITE) {
		printf("\tcopyonwrite = true\n");
	}
	if(serve->expected_size) {
		printf("\tfilesize = %lld\n", (long long int)serve->expected_size);
	}
	if(serve->authname) {
		printf("\tauthfile = %s\n", serve->authname);
	}
	exit(EXIT_SUCCESS);
}
