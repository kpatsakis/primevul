static int nfs41_check_expired_locks(struct nfs4_state *state)
{
	int status, ret = -NFS4ERR_BAD_STATEID;
	struct nfs4_lock_state *lsp;
	struct nfs_server *server = NFS_SERVER(state->inode);

	list_for_each_entry(lsp, &state->lock_states, ls_locks) {
		if (test_bit(NFS_LOCK_INITIALIZED, &lsp->ls_flags)) {
			status = nfs41_test_stateid(server, &lsp->ls_stateid);
			if (status != NFS_OK) {
				/* Free the stateid unless the server
				 * informs us the stateid is unrecognized. */
				if (status != -NFS4ERR_BAD_STATEID)
					nfs41_free_stateid(server,
							&lsp->ls_stateid);
				clear_bit(NFS_LOCK_INITIALIZED, &lsp->ls_flags);
				ret = status;
			}
		}
	};

	return ret;
}
