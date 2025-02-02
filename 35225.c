void serveconnection(CLIENT *client) {
	if(do_run(client->server->prerun, client->exportname)) {
		exit(EXIT_FAILURE);
	}
	setupexport(client);

	if (client->server->flags & F_COPYONWRITE) {
		copyonwrite_prepare(client);
	}

	setmysockopt(client->net);

	mainloop(client);
	do_run(client->server->postrun, client->exportname);
}
