static int rtrs_rdma_route_resolved(struct rtrs_clt_con *con)
{
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);
	struct rtrs_clt_sess *clt = clt_path->clt;
	struct rtrs_msg_conn_req msg;
	struct rdma_conn_param param;

	int err;

	param = (struct rdma_conn_param) {
		.retry_count = 7,
		.rnr_retry_count = 7,
		.private_data = &msg,
		.private_data_len = sizeof(msg),
	};

	msg = (struct rtrs_msg_conn_req) {
		.magic = cpu_to_le16(RTRS_MAGIC),
		.version = cpu_to_le16(RTRS_PROTO_VER),
		.cid = cpu_to_le16(con->c.cid),
		.cid_num = cpu_to_le16(clt_path->s.con_num),
		.recon_cnt = cpu_to_le16(clt_path->s.recon_cnt),
	};
	msg.first_conn = clt_path->for_new_clt ? FIRST_CONN : 0;
	uuid_copy(&msg.sess_uuid, &clt_path->s.uuid);
	uuid_copy(&msg.paths_uuid, &clt->paths_uuid);

	err = rdma_connect_locked(con->c.cm_id, &param);
	if (err)
		rtrs_err(clt, "rdma_connect_locked(): %d\n", err);

	return err;
}