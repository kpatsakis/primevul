static int __init rtrs_client_init(void)
{
	rtrs_rdma_dev_pd_init(0, &dev_pd);

	rtrs_clt_dev_class = class_create(THIS_MODULE, "rtrs-client");
	if (IS_ERR(rtrs_clt_dev_class)) {
		pr_err("Failed to create rtrs-client dev class\n");
		return PTR_ERR(rtrs_clt_dev_class);
	}
	rtrs_wq = alloc_workqueue("rtrs_client_wq", 0, 0);
	if (!rtrs_wq) {
		class_destroy(rtrs_clt_dev_class);
		return -ENOMEM;
	}

	return 0;
}