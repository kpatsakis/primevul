static void uio_free_minor(struct uio_device *idev)
{
	mutex_lock(&minor_lock);
	idr_remove(&uio_idr, idev->minor);
	mutex_unlock(&minor_lock);
}
