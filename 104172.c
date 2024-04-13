static int sapi_fcgi_read_post(char *buffer, uint count_bytes TSRMLS_DC)
{
	uint read_bytes = 0;
	int tmp_read_bytes;
	fcgi_request *request = (fcgi_request*) SG(server_context);

	count_bytes = MIN(count_bytes, (uint) SG(request_info).content_length - SG(read_post_bytes));
	while (read_bytes < count_bytes) {
		tmp_read_bytes = fcgi_read(request, buffer + read_bytes, count_bytes - read_bytes);
		if (tmp_read_bytes <= 0) {
			break;
		}
		read_bytes += tmp_read_bytes;
	}
	return read_bytes;
}
