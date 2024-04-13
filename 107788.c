inline int web_client_api_request_v1_chart(RRDHOST *host, struct web_client *w, char *url) {
     return web_client_api_request_single_chart(host, w, url, rrd_stats_api_v1_chart);
 }
