nfs4_alloc_state_owner(void)
{
	struct nfs4_state_owner *sp;

	sp = kzalloc(sizeof(*sp),GFP_KERNEL);
	if (!sp)
		return NULL;
	spin_lock_init(&sp->so_lock);
	INIT_LIST_HEAD(&sp->so_states);
	INIT_LIST_HEAD(&sp->so_delegations);
	rpc_init_wait_queue(&sp->so_sequence.wait, "Seqid_waitqueue");
	sp->so_seqid.sequence = &sp->so_sequence;
	spin_lock_init(&sp->so_sequence.lock);
	INIT_LIST_HEAD(&sp->so_sequence.list);
	atomic_set(&sp->so_count, 1);
	return sp;
}
