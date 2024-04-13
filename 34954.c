static int cbk_http_request(void *cls,
			    struct MHD_Connection *connection,
			    const char *url,
			    const char *method,
			    const char *version,
			    const char *upload_data,
			    size_t *upload_data_size,
			    void **ptr)
{
	static int dummy;
	struct MHD_Response *response;
	int ret;
	char *nurl;
	unsigned int resp_code;

	if (strcmp(method, "GET"))
		return MHD_NO;

	if (&dummy != *ptr) {
		/* The first time only the headers are valid, do not
		   respond in the first round... */
		*ptr = &dummy;
		return MHD_YES;
	}

	if (*upload_data_size)
		return MHD_NO;

	*ptr = NULL;		/* clear context pointer */

	log_debug(_("HTTP Request: %s"), url);

	nurl = url_normalize(url);

	pmutex_lock(&mutex);
	response = create_response(nurl, method, &resp_code);
	pmutex_unlock(&mutex);

	ret = MHD_queue_response(connection, resp_code, response);
	MHD_destroy_response(response);

	free(nurl);

	return ret;
}
