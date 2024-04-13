static void sapi_fcgi_flush(void *server_context)
{
	fcgi_request *request = (fcgi_request*) server_context;

	if (
#ifndef PHP_WIN32
		!parent &&
#endif
		request && !fcgi_flush(request, 0)) {

		php_handle_aborted_connection();
	}
}
