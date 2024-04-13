static void __put_nfs_open_context(struct nfs_open_context *ctx, int wait)
{
	struct inode *inode;

	if (ctx == NULL)
		return;

	inode = ctx->path.dentry->d_inode;
	if (!atomic_dec_and_lock(&ctx->count, &inode->i_lock))
		return;
	list_del(&ctx->list);
	spin_unlock(&inode->i_lock);
	if (ctx->state != NULL) {
		if (wait)
			nfs4_close_sync(&ctx->path, ctx->state, ctx->mode);
		else
			nfs4_close_state(&ctx->path, ctx->state, ctx->mode);
	}
	if (ctx->cred != NULL)
		put_rpccred(ctx->cred);
	path_put(&ctx->path);
	kfree(ctx);
}
