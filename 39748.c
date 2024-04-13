static int vmci_transport_notify_recv_post_dequeue(
	struct vsock_sock *vsk,
	size_t target,
	ssize_t copied,
	bool data_read,
	struct vsock_transport_recv_notify_data *data)
{
	return vmci_trans(vsk)->notify_ops->recv_post_dequeue(
			&vsk->sk, target, copied, data_read,
			(struct vmci_transport_recv_notify_data *)data);
}
