int rtrs_clt_get_max_reconnect_attempts(const struct rtrs_clt_sess *clt)
{
	return (int)clt->max_reconnect_attempts;
}