static void sapi_cgi_log_message(char *message TSRMLS_DC)
{
	if (fcgi_is_fastcgi() && CGIG(fcgi_logging)) {
		fcgi_request *request;

		request = (fcgi_request*) SG(server_context);
		if (request) {
			int len = strlen(message);
			char *buf = malloc(len+2);

			memcpy(buf, message, len);
			memcpy(buf + len, "\n", sizeof("\n"));
			fcgi_write(request, FCGI_STDERR, buf, len+1);
			free(buf);
		} else {
			fprintf(stderr, "%s\n", message);
		}
		/* ignore return code */
	} else {
		fprintf(stderr, "%s\n", message);
	}
}
