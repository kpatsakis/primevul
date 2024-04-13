static void nfs4_fl_copy_lock(struct file_lock *dst, struct file_lock *src)
{
	struct nfs4_lock_state *lsp = src->fl_u.nfs4_fl.owner;

	dst->fl_u.nfs4_fl.owner = lsp;
	atomic_inc(&lsp->ls_count);
}
