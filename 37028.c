static struct fuse_req *get_reserved_req(struct fuse_conn *fc,
					 struct file *file)
{
	struct fuse_req *req = NULL;
	struct fuse_file *ff = file->private_data;

	do {
		wait_event(fc->reserved_req_waitq, ff->reserved_req);
		spin_lock(&fc->lock);
		if (ff->reserved_req) {
			req = ff->reserved_req;
			ff->reserved_req = NULL;
			get_file(file);
			req->stolen_file = file;
		}
		spin_unlock(&fc->lock);
	} while (!req);

	return req;
}
