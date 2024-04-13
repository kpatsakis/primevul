static char *sapi_fcgi_read_cookies(TSRMLS_D)
{
	fcgi_request *request = (fcgi_request*) SG(server_context);

	return FCGI_GETENV(request, "HTTP_COOKIE");
}
