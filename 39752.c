static int vmci_transport_notify_send_post_enqueue(
	struct vsock_sock *vsk,
	ssize_t written,
	struct vsock_transport_send_notify_data *data)
{
	return vmci_trans(vsk)->notify_ops->send_post_enqueue(
			&vsk->sk, written,
			(struct vmci_transport_send_notify_data *)data);
}
