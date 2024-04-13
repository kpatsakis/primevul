static void nfs4_state_end_reclaim_nograce(struct nfs_client *clp)
{
	clear_bit(NFS4CLNT_RECLAIM_NOGRACE, &clp->cl_state);
}
