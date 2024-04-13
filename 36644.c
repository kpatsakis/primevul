static void nfs_delegation_clear_all(struct nfs_client *clp)
{
	nfs_delegation_mark_reclaim(clp);
	nfs_delegation_reap_unclaimed(clp);
}
