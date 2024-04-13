static void put_nfs_open_context_sync(struct nfs_open_context *ctx)
{
	__put_nfs_open_context(ctx, 1);
}
