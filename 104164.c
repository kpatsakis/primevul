static char *sapi_cgi_read_cookies(TSRMLS_D)
{
	return getenv("HTTP_COOKIE");
}
