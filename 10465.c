static void free_clt(struct rtrs_clt_sess *clt)
{
	free_permits(clt);
	free_percpu(clt->pcpu_path);

	/*
	 * release callback will free clt and destroy mutexes in last put
	 */
	device_unregister(&clt->dev);
}