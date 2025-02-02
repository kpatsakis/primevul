static int request_pending(struct fuse_conn *fc)
{
	return !list_empty(&fc->pending) || !list_empty(&fc->interrupts) ||
		forget_pending(fc);
}
