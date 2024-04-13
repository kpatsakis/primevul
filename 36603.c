static struct nfs4_lock_state *nfs4_alloc_lock_state(struct nfs4_state *state, fl_owner_t fl_owner)
{
	struct nfs4_lock_state *lsp;
	struct nfs_client *clp = state->owner->so_client;

	lsp = kzalloc(sizeof(*lsp), GFP_KERNEL);
	if (lsp == NULL)
		return NULL;
	rpc_init_wait_queue(&lsp->ls_sequence.wait, "lock_seqid_waitqueue");
	spin_lock_init(&lsp->ls_sequence.lock);
	INIT_LIST_HEAD(&lsp->ls_sequence.list);
	lsp->ls_seqid.sequence = &lsp->ls_sequence;
	atomic_set(&lsp->ls_count, 1);
	lsp->ls_owner = fl_owner;
	spin_lock(&clp->cl_lock);
	nfs_alloc_unique_id(&clp->cl_lockowner_id, &lsp->ls_id, 1, 64);
	spin_unlock(&clp->cl_lock);
	INIT_LIST_HEAD(&lsp->ls_locks);
	return lsp;
}
