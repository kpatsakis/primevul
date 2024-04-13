static int nfs41_open_expired(struct nfs4_state_owner *sp, struct nfs4_state *state)
{
	int deleg_status, open_status;
	int deleg_flags = 1 << NFS_DELEGATED_STATE;
	int open_flags = (1 << NFS_O_RDONLY_STATE) | (1 << NFS_O_WRONLY_STATE) | (1 << NFS_O_RDWR_STATE);

	deleg_status = nfs41_check_expired_stateid(state, &state->stateid, deleg_flags);
	open_status = nfs41_check_expired_stateid(state,  &state->open_stateid, open_flags);

	if ((deleg_status == NFS_OK) && (open_status == NFS_OK))
		return NFS_OK;
	return nfs4_open_expired(sp, state);
}
