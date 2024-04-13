void nfs4_copy_stateid(nfs4_stateid *dst, struct nfs4_state *state, fl_owner_t fl_owner)
{
	struct nfs4_lock_state *lsp;
	int seq;

	do {
		seq = read_seqbegin(&state->seqlock);
		memcpy(dst, &state->stateid, sizeof(*dst));
	} while (read_seqretry(&state->seqlock, seq));
	if (test_bit(LK_STATE_IN_USE, &state->flags) == 0)
		return;

	spin_lock(&state->state_lock);
	lsp = __nfs4_find_lock_state(state, fl_owner);
	if (lsp != NULL && (lsp->ls_flags & NFS_LOCK_INITIALIZED) != 0)
		memcpy(dst, &lsp->ls_stateid, sizeof(*dst));
	spin_unlock(&state->state_lock);
	nfs4_put_lock_state(lsp);
}
