static int sapi_cgi_deactivate(TSRMLS_D)
{
	/* flush only when SAPI was started. The reasons are:
		1. SAPI Deactivate is called from two places: module init and request shutdown
		2. When the first call occurs and the request is not set up, flush fails on FastCGI.
	*/
	if (SG(sapi_started)) {
		if (fcgi_is_fastcgi()) {
			if (
#ifndef PHP_WIN32
				!parent &&
#endif
				!fcgi_finish_request((fcgi_request*)SG(server_context), 0)) {
				php_handle_aborted_connection();
			}
		} else {
			sapi_cgi_flush(SG(server_context));
		}
	}
	return SUCCESS;
}
