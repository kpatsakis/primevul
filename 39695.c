void vsock_core_exit(void)
{
	mutex_lock(&vsock_register_mutex);

	misc_deregister(&vsock_device);
	sock_unregister(AF_VSOCK);
	proto_unregister(&vsock_proto);

	/* We do not want the assignment below re-ordered. */
	mb();
	transport = NULL;

	mutex_unlock(&vsock_register_mutex);
}
