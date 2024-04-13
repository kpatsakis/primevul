int vmci_transport_send_read(struct sock *sk)
{
	return vmci_transport_send_control_pkt(
					sk, VMCI_TRANSPORT_PACKET_TYPE_READ, 0,
					0, NULL, VSOCK_PROTO_INVALID,
					VMCI_INVALID_HANDLE);
}
