static int lock_request(struct fuse_conn *fc, struct fuse_req *req)
{
	int err = 0;
	if (req) {
		spin_lock(&fc->lock);
		if (req->aborted)
			err = -ENOENT;
		else
			req->locked = 1;
		spin_unlock(&fc->lock);
	}
	return err;
}
