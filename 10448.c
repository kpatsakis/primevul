static int rtrs_send_path_info(struct rtrs_clt_path *clt_path)
{
	struct rtrs_clt_con *usr_con = to_clt_con(clt_path->s.con[0]);
	struct rtrs_msg_info_req *msg;
	struct rtrs_iu *tx_iu, *rx_iu;
	size_t rx_sz;
	int err;

	rx_sz  = sizeof(struct rtrs_msg_info_rsp);
	rx_sz += sizeof(struct rtrs_sg_desc) * clt_path->queue_depth;

	tx_iu = rtrs_iu_alloc(1, sizeof(struct rtrs_msg_info_req), GFP_KERNEL,
			       clt_path->s.dev->ib_dev, DMA_TO_DEVICE,
			       rtrs_clt_info_req_done);
	rx_iu = rtrs_iu_alloc(1, rx_sz, GFP_KERNEL, clt_path->s.dev->ib_dev,
			       DMA_FROM_DEVICE, rtrs_clt_info_rsp_done);
	if (!tx_iu || !rx_iu) {
		err = -ENOMEM;
		goto out;
	}
	/* Prepare for getting info response */
	err = rtrs_iu_post_recv(&usr_con->c, rx_iu);
	if (err) {
		rtrs_err(clt_path->clt, "rtrs_iu_post_recv(), err: %d\n", err);
		goto out;
	}
	rx_iu = NULL;

	msg = tx_iu->buf;
	msg->type = cpu_to_le16(RTRS_MSG_INFO_REQ);
	memcpy(msg->pathname, clt_path->s.sessname, sizeof(msg->pathname));

	ib_dma_sync_single_for_device(clt_path->s.dev->ib_dev,
				      tx_iu->dma_addr,
				      tx_iu->size, DMA_TO_DEVICE);

	/* Send info request */
	err = rtrs_iu_post_send(&usr_con->c, tx_iu, sizeof(*msg), NULL);
	if (err) {
		rtrs_err(clt_path->clt, "rtrs_iu_post_send(), err: %d\n", err);
		goto out;
	}
	tx_iu = NULL;

	/* Wait for state change */
	wait_event_interruptible_timeout(clt_path->state_wq,
					 clt_path->state != RTRS_CLT_CONNECTING,
					 msecs_to_jiffies(
						 RTRS_CONNECT_TIMEOUT_MS));
	if (READ_ONCE(clt_path->state) != RTRS_CLT_CONNECTED) {
		if (READ_ONCE(clt_path->state) == RTRS_CLT_CONNECTING_ERR)
			err = -ECONNRESET;
		else
			err = -ETIMEDOUT;
	}

out:
	if (tx_iu)
		rtrs_iu_free(tx_iu, clt_path->s.dev->ib_dev, 1);
	if (rx_iu)
		rtrs_iu_free(rx_iu, clt_path->s.dev->ib_dev, 1);
	if (err)
		/* If we've never taken async path because of malloc problems */
		rtrs_clt_change_state_get_old(clt_path,
					      RTRS_CLT_CONNECTING_ERR, NULL);

	return err;
}