create_response_api(const char *nurl, const char *method, unsigned int *rp_code)
{
	struct MHD_Response *resp;
	struct psensor *s;
	char *page = NULL;

	if (!strcmp(nurl, URL_BASE_API_1_1_SENSORS))  {
		page = sensors_to_json_string(server_data.sensors);
#ifdef HAVE_GTOP
	} else if (!strcmp(nurl, URL_API_1_1_SYSINFO)) {
		page = sysinfo_to_json_string(&server_data.psysinfo);
	} else if (!strcmp(nurl, URL_API_1_1_CPU_USAGE)) {
		page = sensor_to_json_string(server_data.cpu_usage);
#endif
	} else if (!strncmp(nurl, URL_BASE_API_1_1_SENSORS,
			    strlen(URL_BASE_API_1_1_SENSORS))
		   && nurl[strlen(URL_BASE_API_1_1_SENSORS)] == '/') {

		const char *sid = nurl + strlen(URL_BASE_API_1_1_SENSORS) + 1;

		s = psensor_list_get_by_id(server_data.sensors, sid);

		if (s)
			page = sensor_to_json_string(s);

	} else if (!strcmp(nurl, URL_API_1_1_SERVER_STOP)) {

		server_stop_requested = 1;
		page = strdup(HTML_STOP_REQUESTED);
	}

	if (page) {
		*rp_code = MHD_HTTP_OK;

		resp = MHD_create_response_from_data(strlen(page), page,
						     MHD_YES, MHD_NO);

		MHD_add_response_header(resp, MHD_HTTP_HEADER_CONTENT_TYPE,
					"application/json");

		return resp;
	}

	return NULL;
}
