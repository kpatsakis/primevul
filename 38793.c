static void nfs4_layoutget_done(struct rpc_task *task, void *calldata)
{
	struct nfs4_layoutget *lgp = calldata;
	struct inode *inode = lgp->args.inode;
	struct nfs_server *server = NFS_SERVER(inode);
	struct pnfs_layout_hdr *lo;
	struct nfs4_state *state = NULL;

	dprintk("--> %s\n", __func__);

	if (!nfs4_sequence_done(task, &lgp->res.seq_res))
		goto out;

	switch (task->tk_status) {
	case 0:
		goto out;
	case -NFS4ERR_LAYOUTTRYLATER:
	case -NFS4ERR_RECALLCONFLICT:
		task->tk_status = -NFS4ERR_DELAY;
		break;
	case -NFS4ERR_EXPIRED:
	case -NFS4ERR_BAD_STATEID:
		spin_lock(&inode->i_lock);
		lo = NFS_I(inode)->layout;
		if (!lo || list_empty(&lo->plh_segs)) {
			spin_unlock(&inode->i_lock);
			/* If the open stateid was bad, then recover it. */
			state = lgp->args.ctx->state;
		} else {
			LIST_HEAD(head);

			pnfs_mark_matching_lsegs_invalid(lo, &head, NULL);
			spin_unlock(&inode->i_lock);
			/* Mark the bad layout state as invalid, then
			 * retry using the open stateid. */
			pnfs_free_lseg_list(&head);
		}
	}
	if (nfs4_async_handle_error(task, server, state) == -EAGAIN)
		rpc_restart_call_prepare(task);
out:
	dprintk("<-- %s\n", __func__);
}
