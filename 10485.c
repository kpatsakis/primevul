static int init_path(struct rtrs_clt_path *clt_path)
{
	int err;
	char str[NAME_MAX];
	struct rtrs_addr path = {
		.src = &clt_path->s.src_addr,
		.dst = &clt_path->s.dst_addr,
	};

	rtrs_addr_to_str(&path, str, sizeof(str));

	mutex_lock(&clt_path->init_mutex);
	err = init_conns(clt_path);
	if (err) {
		rtrs_err(clt_path->clt,
			 "init_conns() failed: err=%d path=%s [%s:%u]\n", err,
			 str, clt_path->hca_name, clt_path->hca_port);
		goto out;
	}
	err = rtrs_send_path_info(clt_path);
	if (err) {
		rtrs_err(clt_path->clt,
			 "rtrs_send_path_info() failed: err=%d path=%s [%s:%u]\n",
			 err, str, clt_path->hca_name, clt_path->hca_port);
		goto out;
	}
	rtrs_clt_path_up(clt_path);
out:
	mutex_unlock(&clt_path->init_mutex);

	return err;
}