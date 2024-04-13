void serve_err(SERVER* serve, const char* msg) {
	g_message("Export of %s on port %d failed:", serve->exportname,
			serve->port);
	err(msg);
}
