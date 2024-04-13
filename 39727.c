__vmci_transport_send_control_pkt(struct vmci_transport_packet *pkt,
				  struct sockaddr_vm *src,
				  struct sockaddr_vm *dst,
				  enum vmci_transport_packet_type type,
				  u64 size,
				  u64 mode,
				  struct vmci_transport_waiting_info *wait,
				  u16 proto,
				  struct vmci_handle handle,
				  bool convert_error)
{
	int err;

	vmci_transport_packet_init(pkt, src, dst, type, size, mode, wait,
				   proto, handle);
	err = vmci_datagram_send(&pkt->dg);
	if (convert_error && (err < 0))
		return vmci_transport_error_to_vsock_error(err);

	return err;
}
