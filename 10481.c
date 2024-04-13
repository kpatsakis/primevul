static struct rtrs_clt_path *alloc_path(struct rtrs_clt_sess *clt,
					const struct rtrs_addr *path,
					size_t con_num, u32 nr_poll_queues)
{
	struct rtrs_clt_path *clt_path;
	int err = -ENOMEM;
	int cpu;
	size_t total_con;

	clt_path = kzalloc(sizeof(*clt_path), GFP_KERNEL);
	if (!clt_path)
		goto err;

	/*
	 * irqmode and poll
	 * +1: Extra connection for user messages
	 */
	total_con = con_num + nr_poll_queues + 1;
	clt_path->s.con = kcalloc(total_con, sizeof(*clt_path->s.con),
				  GFP_KERNEL);
	if (!clt_path->s.con)
		goto err_free_path;

	clt_path->s.con_num = total_con;
	clt_path->s.irq_con_num = con_num + 1;

	clt_path->stats = kzalloc(sizeof(*clt_path->stats), GFP_KERNEL);
	if (!clt_path->stats)
		goto err_free_con;

	mutex_init(&clt_path->init_mutex);
	uuid_gen(&clt_path->s.uuid);
	memcpy(&clt_path->s.dst_addr, path->dst,
	       rdma_addr_size((struct sockaddr *)path->dst));

	/*
	 * rdma_resolve_addr() passes src_addr to cma_bind_addr, which
	 * checks the sa_family to be non-zero. If user passed src_addr=NULL
	 * the sess->src_addr will contain only zeros, which is then fine.
	 */
	if (path->src)
		memcpy(&clt_path->s.src_addr, path->src,
		       rdma_addr_size((struct sockaddr *)path->src));
	strscpy(clt_path->s.sessname, clt->sessname,
		sizeof(clt_path->s.sessname));
	clt_path->clt = clt;
	clt_path->max_pages_per_mr = RTRS_MAX_SEGMENTS;
	init_waitqueue_head(&clt_path->state_wq);
	clt_path->state = RTRS_CLT_CONNECTING;
	atomic_set(&clt_path->connected_cnt, 0);
	INIT_WORK(&clt_path->close_work, rtrs_clt_close_work);
	INIT_DELAYED_WORK(&clt_path->reconnect_dwork, rtrs_clt_reconnect_work);
	rtrs_clt_init_hb(clt_path);

	clt_path->mp_skip_entry = alloc_percpu(typeof(*clt_path->mp_skip_entry));
	if (!clt_path->mp_skip_entry)
		goto err_free_stats;

	for_each_possible_cpu(cpu)
		INIT_LIST_HEAD(per_cpu_ptr(clt_path->mp_skip_entry, cpu));

	err = rtrs_clt_init_stats(clt_path->stats);
	if (err)
		goto err_free_percpu;

	return clt_path;

err_free_percpu:
	free_percpu(clt_path->mp_skip_entry);
err_free_stats:
	kfree(clt_path->stats);
err_free_con:
	kfree(clt_path->s.con);
err_free_path:
	kfree(clt_path);
err:
	return ERR_PTR(err);
}