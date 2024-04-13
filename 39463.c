static void vhost_net_zerocopy_done_signal(struct kref *kref)
{
	struct vhost_net_ubuf_ref *ubufs;

	ubufs = container_of(kref, struct vhost_net_ubuf_ref, kref);
	wake_up(&ubufs->wait);
}
