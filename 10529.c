static struct rtrs_clt_sess *alloc_clt(const char *sessname, size_t paths_num,
				  u16 port, size_t pdu_sz, void *priv,
				  void	(*link_ev)(void *priv,
						   enum rtrs_clt_link_ev ev),
				  unsigned int reconnect_delay_sec,
				  unsigned int max_reconnect_attempts)
{
	struct rtrs_clt_sess *clt;
	int err;

	if (!paths_num || paths_num > MAX_PATHS_NUM)
		return ERR_PTR(-EINVAL);

	if (strlen(sessname) >= sizeof(clt->sessname))
		return ERR_PTR(-EINVAL);

	clt = kzalloc(sizeof(*clt), GFP_KERNEL);
	if (!clt)
		return ERR_PTR(-ENOMEM);

	clt->pcpu_path = alloc_percpu(typeof(*clt->pcpu_path));
	if (!clt->pcpu_path) {
		kfree(clt);
		return ERR_PTR(-ENOMEM);
	}

	clt->dev.class = rtrs_clt_dev_class;
	clt->dev.release = rtrs_clt_dev_release;
	uuid_gen(&clt->paths_uuid);
	INIT_LIST_HEAD_RCU(&clt->paths_list);
	clt->paths_num = paths_num;
	clt->paths_up = MAX_PATHS_NUM;
	clt->port = port;
	clt->pdu_sz = pdu_sz;
	clt->max_segments = RTRS_MAX_SEGMENTS;
	clt->reconnect_delay_sec = reconnect_delay_sec;
	clt->max_reconnect_attempts = max_reconnect_attempts;
	clt->priv = priv;
	clt->link_ev = link_ev;
	clt->mp_policy = MP_POLICY_MIN_INFLIGHT;
	strscpy(clt->sessname, sessname, sizeof(clt->sessname));
	init_waitqueue_head(&clt->permits_wait);
	mutex_init(&clt->paths_ev_mutex);
	mutex_init(&clt->paths_mutex);
	device_initialize(&clt->dev);

	err = dev_set_name(&clt->dev, "%s", sessname);
	if (err)
		goto err_put;

	/*
	 * Suppress user space notification until
	 * sysfs files are created
	 */
	dev_set_uevent_suppress(&clt->dev, true);
	err = device_add(&clt->dev);
	if (err)
		goto err_put;

	clt->kobj_paths = kobject_create_and_add("paths", &clt->dev.kobj);
	if (!clt->kobj_paths) {
		err = -ENOMEM;
		goto err_del;
	}
	err = rtrs_clt_create_sysfs_root_files(clt);
	if (err) {
		kobject_del(clt->kobj_paths);
		kobject_put(clt->kobj_paths);
		goto err_del;
	}
	dev_set_uevent_suppress(&clt->dev, false);
	kobject_uevent(&clt->dev.kobj, KOBJ_ADD);

	return clt;
err_del:
	device_del(&clt->dev);
err_put:
	free_percpu(clt->pcpu_path);
	put_device(&clt->dev);
	return ERR_PTR(err);
}