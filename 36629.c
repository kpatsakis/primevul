static int nfs4_reclaim_locks(struct nfs4_state *state, const struct nfs4_state_recovery_ops *ops)
{
	struct inode *inode = state->inode;
	struct nfs_inode *nfsi = NFS_I(inode);
	struct file_lock *fl;
	int status = 0;

	down_write(&nfsi->rwsem);
	for (fl = inode->i_flock; fl != NULL; fl = fl->fl_next) {
		if (!(fl->fl_flags & (FL_POSIX|FL_FLOCK)))
			continue;
		if (nfs_file_open_context(fl->fl_file)->state != state)
			continue;
		status = ops->recover_lock(state, fl);
		if (status >= 0)
			continue;
		switch (status) {
			default:
				printk(KERN_ERR "%s: unhandled error %d. Zeroing state\n",
						__func__, status);
			case -NFS4ERR_EXPIRED:
			case -NFS4ERR_NO_GRACE:
			case -NFS4ERR_RECLAIM_BAD:
			case -NFS4ERR_RECLAIM_CONFLICT:
				/* kill_proc(fl->fl_pid, SIGLOST, 1); */
				break;
			case -NFS4ERR_STALE_CLIENTID:
				goto out_err;
		}
	}
	up_write(&nfsi->rwsem);
	return 0;
out_err:
	up_write(&nfsi->rwsem);
	return status;
}
