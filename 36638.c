static void nfs4_state_end_reclaim_reboot(struct nfs_client *clp)
{
	struct nfs4_state_owner *sp;
	struct rb_node *pos;
	struct nfs4_state *state;

	if (!test_and_clear_bit(NFS4CLNT_RECLAIM_REBOOT, &clp->cl_state))
		return;

	for (pos = rb_first(&clp->cl_state_owners); pos != NULL; pos = rb_next(pos)) {
		sp = rb_entry(pos, struct nfs4_state_owner, so_client_node);
		spin_lock(&sp->so_lock);
		list_for_each_entry(state, &sp->so_states, open_states) {
			if (!test_and_clear_bit(NFS_STATE_RECLAIM_REBOOT, &state->flags))
				continue;
			nfs4_state_mark_reclaim_nograce(clp, state);
		}
		spin_unlock(&sp->so_lock);
	}

	nfs_delegation_reap_unclaimed(clp);
}
