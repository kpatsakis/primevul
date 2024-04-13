static int vmci_transport_send_qp_offer(struct sock *sk,
					struct vmci_handle handle)
{
	return vmci_transport_send_control_pkt(
					sk, VMCI_TRANSPORT_PACKET_TYPE_OFFER, 0,
					0, NULL,
					VSOCK_PROTO_INVALID, handle);
}
