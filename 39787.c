int vmci_transport_send_wrote(struct sock *sk)
{
	return vmci_transport_send_control_pkt(
					sk, VMCI_TRANSPORT_PACKET_TYPE_WROTE, 0,
					0, NULL, VSOCK_PROTO_INVALID,
					VMCI_INVALID_HANDLE);
}
