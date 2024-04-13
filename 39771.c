static int vmci_transport_reply_reset(struct vmci_transport_packet *pkt)
{
	return vmci_transport_reply_control_pkt_fast(
						pkt,
						VMCI_TRANSPORT_PACKET_TYPE_RST,
						0, 0, NULL,
						VMCI_INVALID_HANDLE);
}
