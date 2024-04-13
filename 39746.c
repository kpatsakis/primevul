static int vmci_transport_notify_poll_out(
	struct vsock_sock *vsk,
	size_t target,
	bool *space_available_now)
{
	return vmci_trans(vsk)->notify_ops->poll_out(
			&vsk->sk, target, space_available_now);
}
