static void nfs4_layoutget_release(void *calldata)
{
	struct nfs4_layoutget *lgp = calldata;

	dprintk("--> %s\n", __func__);
	put_nfs_open_context(lgp->args.ctx);
	kfree(calldata);
	dprintk("<-- %s\n", __func__);
}
