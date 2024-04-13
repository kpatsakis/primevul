static int vmci_transport_notify_recv_pre_dequeue(
	struct vsock_sock *vsk,
	size_t target,
	struct vsock_transport_recv_notify_data *data)
{
	return vmci_trans(vsk)->notify_ops->recv_pre_dequeue(
			&vsk->sk, target,
			(struct vmci_transport_recv_notify_data *)data);
}
