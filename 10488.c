void rtrs_clt_set_max_reconnect_attempts(struct rtrs_clt_sess *clt, int value)
{
	clt->max_reconnect_attempts = (unsigned int)value;
}