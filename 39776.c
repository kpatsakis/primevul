vmci_transport_send_control_pkt_bh(struct sockaddr_vm *src,
				   struct sockaddr_vm *dst,
				   enum vmci_transport_packet_type type,
				   u64 size,
				   u64 mode,
				   struct vmci_transport_waiting_info *wait,
				   struct vmci_handle handle)
{
	/* Note that it is safe to use a single packet across all CPUs since
	 * two tasklets of the same type are guaranteed to not ever run
	 * simultaneously. If that ever changes, or VMCI stops using tasklets,
	 * we can use per-cpu packets.
	 */
	static struct vmci_transport_packet pkt;

	return __vmci_transport_send_control_pkt(&pkt, src, dst, type,
						 size, mode, wait,
						 VSOCK_PROTO_INVALID, handle,
						 false);
}
