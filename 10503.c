static int rtrs_rdma_conn_established(struct rtrs_clt_con *con,
				       struct rdma_cm_event *ev)
{
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);
	struct rtrs_clt_sess *clt = clt_path->clt;
	const struct rtrs_msg_conn_rsp *msg;
	u16 version, queue_depth;
	int errno;
	u8 len;

	msg = ev->param.conn.private_data;
	len = ev->param.conn.private_data_len;
	if (len < sizeof(*msg)) {
		rtrs_err(clt, "Invalid RTRS connection response\n");
		return -ECONNRESET;
	}
	if (le16_to_cpu(msg->magic) != RTRS_MAGIC) {
		rtrs_err(clt, "Invalid RTRS magic\n");
		return -ECONNRESET;
	}
	version = le16_to_cpu(msg->version);
	if (version >> 8 != RTRS_PROTO_VER_MAJOR) {
		rtrs_err(clt, "Unsupported major RTRS version: %d, expected %d\n",
			  version >> 8, RTRS_PROTO_VER_MAJOR);
		return -ECONNRESET;
	}
	errno = le16_to_cpu(msg->errno);
	if (errno) {
		rtrs_err(clt, "Invalid RTRS message: errno %d\n",
			  errno);
		return -ECONNRESET;
	}
	if (con->c.cid == 0) {
		queue_depth = le16_to_cpu(msg->queue_depth);

		if (clt_path->queue_depth > 0 && queue_depth != clt_path->queue_depth) {
			rtrs_err(clt, "Error: queue depth changed\n");

			/*
			 * Stop any more reconnection attempts
			 */
			clt_path->reconnect_attempts = -1;
			rtrs_err(clt,
				"Disabling auto-reconnect. Trigger a manual reconnect after issue is resolved\n");
			return -ECONNRESET;
		}

		if (!clt_path->rbufs) {
			clt_path->rbufs = kcalloc(queue_depth,
						  sizeof(*clt_path->rbufs),
						  GFP_KERNEL);
			if (!clt_path->rbufs)
				return -ENOMEM;
		}
		clt_path->queue_depth = queue_depth;
		clt_path->s.signal_interval = min_not_zero(queue_depth,
						(unsigned short) SERVICE_CON_QUEUE_DEPTH);
		clt_path->max_hdr_size = le32_to_cpu(msg->max_hdr_size);
		clt_path->max_io_size = le32_to_cpu(msg->max_io_size);
		clt_path->flags = le32_to_cpu(msg->flags);
		clt_path->chunk_size = clt_path->max_io_size + clt_path->max_hdr_size;

		/*
		 * Global IO size is always a minimum.
		 * If while a reconnection server sends us a value a bit
		 * higher - client does not care and uses cached minimum.
		 *
		 * Since we can have several sessions (paths) restablishing
		 * connections in parallel, use lock.
		 */
		mutex_lock(&clt->paths_mutex);
		clt->queue_depth = clt_path->queue_depth;
		clt->max_io_size = min_not_zero(clt_path->max_io_size,
						clt->max_io_size);
		mutex_unlock(&clt->paths_mutex);

		/*
		 * Cache the hca_port and hca_name for sysfs
		 */
		clt_path->hca_port = con->c.cm_id->port_num;
		scnprintf(clt_path->hca_name, sizeof(clt_path->hca_name),
			  clt_path->s.dev->ib_dev->name);
		clt_path->s.src_addr = con->c.cm_id->route.addr.src_addr;
		/* set for_new_clt, to allow future reconnect on any path */
		clt_path->for_new_clt = 1;
	}

	return 0;
}