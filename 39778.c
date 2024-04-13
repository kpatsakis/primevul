static int vmci_transport_send_negotiate(struct sock *sk, size_t size)
{
	return vmci_transport_send_control_pkt(
					sk,
					VMCI_TRANSPORT_PACKET_TYPE_NEGOTIATE,
					size, 0, NULL,
					VSOCK_PROTO_INVALID,
					VMCI_INVALID_HANDLE);
}
