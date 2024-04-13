static char *sapi_cgi_getenv(char *name, size_t name_len TSRMLS_DC)
{
	return getenv(name);
}
