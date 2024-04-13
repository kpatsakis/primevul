void daemonize(SERVER* serve) {
	FILE*pidf;

	if(serve && !(serve->port)) {
		return;
	}
	if(daemon(0,0)<0) {
		err("daemon");
	}
	if(!*pidftemplate) {
		if(serve) {
			strncpy(pidftemplate, "/var/run/nbd-server.%d.pid", 255);
		} else {
			strncpy(pidftemplate, "/var/run/nbd-server.pid", 255);
		}
	}
	snprintf(pidfname, 255, pidftemplate, serve ? serve->port : 0);
	pidf=fopen(pidfname, "w");
	if(pidf) {
		fprintf(pidf,"%d\n", (int)getpid());
		fclose(pidf);
	} else {
		perror("fopen");
		fprintf(stderr, "Not fatal; continuing");
	}
}
