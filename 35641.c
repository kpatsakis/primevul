static void nfs4_free_reclaim_complete_data(void *data)
{
	struct nfs4_reclaim_complete_data *calldata = data;

	kfree(calldata);
}
