static int nfs4_do_reclaim(struct nfs_client *clp, const struct nfs4_state_recovery_ops *ops)
{
	struct rb_node *pos;
	int status = 0;

restart:
	spin_lock(&clp->cl_lock);
	for (pos = rb_first(&clp->cl_state_owners); pos != NULL; pos = rb_next(pos)) {
		struct nfs4_state_owner *sp = rb_entry(pos, struct nfs4_state_owner, so_client_node);
		if (!test_and_clear_bit(ops->owner_flag_bit, &sp->so_flags))
			continue;
		atomic_inc(&sp->so_count);
		spin_unlock(&clp->cl_lock);
		status = nfs4_reclaim_open_state(sp, ops);
		if (status < 0) {
			set_bit(ops->owner_flag_bit, &sp->so_flags);
			nfs4_put_state_owner(sp);
			nfs4_recovery_handle_error(clp, status);
			return status;
		}
		nfs4_put_state_owner(sp);
		goto restart;
	}
	spin_unlock(&clp->cl_lock);
	return status;
}
