static void nfs4_fl_release_lock(struct file_lock *fl)
{
	nfs4_put_lock_state(fl->fl_u.nfs4_fl.owner);
}
