static void rtrs_clt_path_down(struct rtrs_clt_path *clt_path)
{
	struct rtrs_clt_sess *clt = clt_path->clt;

	if (!clt_path->established)
		return;

	clt_path->established = false;
	mutex_lock(&clt->paths_ev_mutex);
	WARN_ON(!clt->paths_up);
	if (--clt->paths_up == 0)
		clt->link_ev(clt->priv, RTRS_CLT_LINK_EV_DISCONNECTED);
	mutex_unlock(&clt->paths_ev_mutex);
}