int vsock_core_init(const struct vsock_transport *t)
{
	int retval = mutex_lock_interruptible(&vsock_register_mutex);
	if (retval)
		return retval;

	if (transport) {
		retval = -EBUSY;
		goto out;
	}

	transport = t;
	retval = __vsock_core_init();
	if (retval)
		transport = NULL;

out:
	mutex_unlock(&vsock_register_mutex);
	return retval;
}
