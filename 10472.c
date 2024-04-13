void free_path(struct rtrs_clt_path *clt_path)
{
	free_percpu(clt_path->mp_skip_entry);
	mutex_destroy(&clt_path->init_mutex);
	kfree(clt_path->s.con);
	kfree(clt_path->rbufs);
	kfree(clt_path);
}