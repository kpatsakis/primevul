void remove_server(gpointer s) {
	SERVER *server;

	server=(SERVER*)s;
	g_free(server->exportname);
	if(server->authname)
		g_free(server->authname);
	if(server->listenaddr)
		g_free(server->listenaddr);
	if(server->prerun)
		g_free(server->prerun);
	if(server->postrun)
		g_free(server->postrun);
	g_free(server);
}
