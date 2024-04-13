static void nfs4_state_start_reclaim_nograce(struct nfs_client *clp)
{
	nfs_delegation_clear_all(clp);
	nfs4_state_mark_reclaim_helper(clp, nfs4_state_mark_reclaim_nograce);
}
