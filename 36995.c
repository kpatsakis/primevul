static ssize_t fuse_dev_do_write(struct fuse_conn *fc,
				 struct fuse_copy_state *cs, size_t nbytes)
{
	int err;
	struct fuse_req *req;
	struct fuse_out_header oh;

	if (nbytes < sizeof(struct fuse_out_header))
		return -EINVAL;

	err = fuse_copy_one(cs, &oh, sizeof(oh));
	if (err)
		goto err_finish;

	err = -EINVAL;
	if (oh.len != nbytes)
		goto err_finish;

	/*
	 * Zero oh.unique indicates unsolicited notification message
	 * and error contains notification code.
	 */
	if (!oh.unique) {
		err = fuse_notify(fc, oh.error, nbytes - sizeof(oh), cs);
		return err ? err : nbytes;
	}

	err = -EINVAL;
	if (oh.error <= -1000 || oh.error > 0)
		goto err_finish;

	spin_lock(&fc->lock);
	err = -ENOENT;
	if (!fc->connected)
		goto err_unlock;

	req = request_find(fc, oh.unique);
	if (!req)
		goto err_unlock;

	if (req->aborted) {
		spin_unlock(&fc->lock);
		fuse_copy_finish(cs);
		spin_lock(&fc->lock);
		request_end(fc, req);
		return -ENOENT;
	}
	/* Is it an interrupt reply? */
	if (req->intr_unique == oh.unique) {
		err = -EINVAL;
		if (nbytes != sizeof(struct fuse_out_header))
			goto err_unlock;

		if (oh.error == -ENOSYS)
			fc->no_interrupt = 1;
		else if (oh.error == -EAGAIN)
			queue_interrupt(fc, req);

		spin_unlock(&fc->lock);
		fuse_copy_finish(cs);
		return nbytes;
	}

	req->state = FUSE_REQ_WRITING;
	list_move(&req->list, &fc->io);
	req->out.h = oh;
	req->locked = 1;
	cs->req = req;
	if (!req->out.page_replace)
		cs->move_pages = 0;
	spin_unlock(&fc->lock);

	err = copy_out_args(cs, &req->out, nbytes);
	fuse_copy_finish(cs);

	spin_lock(&fc->lock);
	req->locked = 0;
	if (!err) {
		if (req->aborted)
			err = -ENOENT;
	} else if (!req->aborted)
		req->out.h.error = -EIO;
	request_end(fc, req);

	return err ? err : nbytes;

 err_unlock:
	spin_unlock(&fc->lock);
 err_finish:
	fuse_copy_finish(cs);
	return err;
}
