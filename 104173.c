static int sapi_fcgi_ub_write(const char *str, uint str_length TSRMLS_DC)
{
	const char *ptr = str;
	uint remaining = str_length;
	fcgi_request *request = (fcgi_request*) SG(server_context);

	while (remaining > 0) {
		long ret = fcgi_write(request, FCGI_STDOUT, ptr, remaining);

		if (ret <= 0) {
			php_handle_aborted_connection();
			return str_length - remaining;
		}
		ptr += ret;
		remaining -= ret;
	}

	return str_length;
}
