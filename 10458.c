static int rtrs_post_recv_empty_x2(struct rtrs_con *con, struct ib_cqe *cqe)
{
	struct ib_recv_wr wr_arr[2], *wr;
	int i;

	memset(wr_arr, 0, sizeof(wr_arr));
	for (i = 0; i < ARRAY_SIZE(wr_arr); i++) {
		wr = &wr_arr[i];
		wr->wr_cqe  = cqe;
		if (i)
			/* Chain backwards */
			wr->next = &wr_arr[i - 1];
	}

	return ib_post_recv(con->qp, wr, NULL);
}