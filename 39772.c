static int vmci_transport_send_attach(struct sock *sk,
				      struct vmci_handle handle)
{
	return vmci_transport_send_control_pkt(
					sk, VMCI_TRANSPORT_PACKET_TYPE_ATTACH,
					0, 0, NULL, VSOCK_PROTO_INVALID,
					handle);
}
