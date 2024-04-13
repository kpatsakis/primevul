static char *sapi_fcgi_getenv(char *name, size_t name_len TSRMLS_DC)
{
	/* when php is started by mod_fastcgi, no regular environment
	 * is provided to PHP.  It is always sent to PHP at the start
	 * of a request.  So we have to do our own lookup to get env
	 * vars.  This could probably be faster somehow.  */
	fcgi_request *request = (fcgi_request*) SG(server_context);
	char *ret = fcgi_getenv(request, name, name_len);

	if (ret) return ret;
	/*  if cgi, or fastcgi and not found in fcgi env
		check the regular environment */
	return getenv(name);
}
