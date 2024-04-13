nfs4_proc_create(struct inode *dir, struct dentry *dentry, struct iattr *sattr,
                 int flags, struct nfs_open_context *ctx)
{
	struct dentry *de = dentry;
	struct nfs4_state *state;
	struct rpc_cred *cred = NULL;
	fmode_t fmode = 0;
	int status = 0;

	if (ctx != NULL) {
		cred = ctx->cred;
		de = ctx->dentry;
		fmode = ctx->mode;
	}
	sattr->ia_mode &= ~current_umask();
	state = nfs4_do_open(dir, de, fmode, flags, sattr, cred);
	d_drop(dentry);
	if (IS_ERR(state)) {
		status = PTR_ERR(state);
		goto out;
	}
	d_add(dentry, igrab(state->inode));
	nfs_set_verifier(dentry, nfs_save_change_attribute(dir));
	if (ctx != NULL)
		ctx->state = state;
	else
		nfs4_close_sync(state, fmode);
out:
	return status;
}
