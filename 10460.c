static int post_recv_io(struct rtrs_clt_con *con, size_t q_size)
{
	int err, i;
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);

	for (i = 0; i < q_size; i++) {
		if (clt_path->flags & RTRS_MSG_NEW_RKEY_F) {
			struct rtrs_iu *iu = &con->rsp_ius[i];

			err = rtrs_iu_post_recv(&con->c, iu);
		} else {
			err = rtrs_post_recv_empty(&con->c, &io_comp_cqe);
		}
		if (err)
			return err;
	}

	return 0;
}