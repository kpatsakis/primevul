static int vmci_transport_shutdown(struct vsock_sock *vsk, int mode)
{
	return vmci_transport_send_control_pkt(
					&vsk->sk,
					VMCI_TRANSPORT_PACKET_TYPE_SHUTDOWN,
					0, mode, NULL,
					VSOCK_PROTO_INVALID,
					VMCI_INVALID_HANDLE);
}
