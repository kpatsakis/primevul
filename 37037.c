static void unlock_request(struct fuse_conn *fc, struct fuse_req *req)
{
	if (req) {
		spin_lock(&fc->lock);
		req->locked = 0;
		if (req->aborted)
			wake_up(&req->waitq);
		spin_unlock(&fc->lock);
	}
}
