static int nfs41_check_expired_stateid(struct nfs4_state *state, nfs4_stateid *stateid, unsigned int flags)
{
	int status = NFS_OK;
	struct nfs_server *server = NFS_SERVER(state->inode);

	if (state->flags & flags) {
		status = nfs41_test_stateid(server, stateid);
		if (status != NFS_OK) {
			nfs41_free_stateid(server, stateid);
			state->flags &= ~flags;
		}
	}
	return status;
}
