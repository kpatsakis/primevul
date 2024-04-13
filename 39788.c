int vmci_transport_send_wrote_bh(struct sockaddr_vm *dst,
				 struct sockaddr_vm *src)
{
	return vmci_transport_send_control_pkt_bh(
					dst, src,
					VMCI_TRANSPORT_PACKET_TYPE_WROTE, 0,
					0, NULL, VMCI_INVALID_HANDLE);
}
