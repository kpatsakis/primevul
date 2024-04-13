vmci_transport_packet_get_addresses(struct vmci_transport_packet *pkt,
				    struct sockaddr_vm *local,
				    struct sockaddr_vm *remote)
{
	vsock_addr_init(local, pkt->dg.dst.context, pkt->dst_port);
	vsock_addr_init(remote, pkt->dg.src.context, pkt->src_port);
}
