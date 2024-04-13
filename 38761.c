static void nfs41_clear_delegation_stateid(struct nfs4_state *state)
{
	struct nfs_server *server = NFS_SERVER(state->inode);
	nfs4_stateid *stateid = &state->stateid;
	int status;

	/* If a state reset has been done, test_stateid is unneeded */
	if (test_bit(NFS_DELEGATED_STATE, &state->flags) == 0)
		return;

	status = nfs41_test_stateid(server, stateid);
	if (status != NFS_OK) {
		/* Free the stateid unless the server explicitly
		 * informs us the stateid is unrecognized. */
		if (status != -NFS4ERR_BAD_STATEID)
			nfs41_free_stateid(server, stateid);
		nfs_remove_bad_delegation(state->inode);

		write_seqlock(&state->seqlock);
		nfs4_stateid_copy(&state->stateid, &state->open_stateid);
		write_sequnlock(&state->seqlock);
		clear_bit(NFS_DELEGATED_STATE, &state->flags);
	}
}
