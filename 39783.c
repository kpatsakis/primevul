static int vmci_transport_send_reset(struct sock *sk,
				     struct vmci_transport_packet *pkt)
{
	if (pkt->type == VMCI_TRANSPORT_PACKET_TYPE_RST)
		return 0;
	return vmci_transport_send_control_pkt(sk,
					VMCI_TRANSPORT_PACKET_TYPE_RST,
					0, 0, NULL, VSOCK_PROTO_INVALID,
					VMCI_INVALID_HANDLE);
}
