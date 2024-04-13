__nfs4_find_lock_state(struct nfs4_state *state, fl_owner_t fl_owner)
{
	struct nfs4_lock_state *pos;
	list_for_each_entry(pos, &state->lock_states, ls_locks) {
		if (pos->ls_owner != fl_owner)
			continue;
		atomic_inc(&pos->ls_count);
		return pos;
	}
	return NULL;
}
