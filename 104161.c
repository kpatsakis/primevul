static void sapi_cgi_flush(void *server_context)
{
	if (fflush(stdout) == EOF) {
		php_handle_aborted_connection();
	}
}
