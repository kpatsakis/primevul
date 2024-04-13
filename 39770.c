vmci_transport_reply_control_pkt_fast(struct vmci_transport_packet *pkt,
				      enum vmci_transport_packet_type type,
				      u64 size,
				      u64 mode,
				      struct vmci_transport_waiting_info *wait,
				      struct vmci_handle handle)
{
	struct vmci_transport_packet reply;
	struct sockaddr_vm src, dst;

	if (pkt->type == VMCI_TRANSPORT_PACKET_TYPE_RST) {
		return 0;
	} else {
		vmci_transport_packet_get_addresses(pkt, &src, &dst);
		return __vmci_transport_send_control_pkt(&reply, &src, &dst,
							 type,
							 size, mode, wait,
							 VSOCK_PROTO_INVALID,
							 handle, true);
	}
}
