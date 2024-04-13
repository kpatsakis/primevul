static int nfs4_state_mark_reclaim_reboot(struct nfs_client *clp, struct nfs4_state *state)
{

	set_bit(NFS_STATE_RECLAIM_REBOOT, &state->flags);
	/* Don't recover state that expired before the reboot */
	if (test_bit(NFS_STATE_RECLAIM_NOGRACE, &state->flags)) {
		clear_bit(NFS_STATE_RECLAIM_REBOOT, &state->flags);
		return 0;
	}
	set_bit(NFS_OWNER_RECLAIM_REBOOT, &state->owner->so_flags);
	set_bit(NFS4CLNT_RECLAIM_REBOOT, &clp->cl_state);
	return 1;
}
