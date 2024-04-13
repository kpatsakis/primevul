static int vmci_transport_send_reset_bh(struct sockaddr_vm *dst,
					struct sockaddr_vm *src,
					struct vmci_transport_packet *pkt)
{
	if (pkt->type == VMCI_TRANSPORT_PACKET_TYPE_RST)
		return 0;
	return vmci_transport_send_control_pkt_bh(
					dst, src,
					VMCI_TRANSPORT_PACKET_TYPE_RST, 0,
					0, NULL, VMCI_INVALID_HANDLE);
}
