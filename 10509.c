static void __exit rtrs_client_exit(void)
{
	destroy_workqueue(rtrs_wq);
	class_destroy(rtrs_clt_dev_class);
	rtrs_rdma_dev_pd_deinit(&dev_pd);
}