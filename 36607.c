static void nfs4_clear_open_state(struct nfs4_state *state)
{
	struct nfs4_lock_state *lock;

	clear_bit(NFS_DELEGATED_STATE, &state->flags);
	clear_bit(NFS_O_RDONLY_STATE, &state->flags);
	clear_bit(NFS_O_WRONLY_STATE, &state->flags);
	clear_bit(NFS_O_RDWR_STATE, &state->flags);
	list_for_each_entry(lock, &state->lock_states, ls_locks) {
		lock->ls_seqid.flags = 0;
		lock->ls_flags &= ~NFS_LOCK_INITIALIZED;
	}
}
