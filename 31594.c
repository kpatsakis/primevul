static const int http_get_status_idx(unsigned int status)
{
	switch (status) {
	case 200: return HTTP_ERR_200;
	case 400: return HTTP_ERR_400;
	case 403: return HTTP_ERR_403;
	case 405: return HTTP_ERR_405;
	case 408: return HTTP_ERR_408;
	case 425: return HTTP_ERR_425;
	case 429: return HTTP_ERR_429;
	case 500: return HTTP_ERR_500;
	case 502: return HTTP_ERR_502;
	case 503: return HTTP_ERR_503;
	case 504: return HTTP_ERR_504;
	default: return HTTP_ERR_500;
	}
}
