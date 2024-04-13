static int __vsock_bind_dgram(struct vsock_sock *vsk,
			      struct sockaddr_vm *addr)
{
	return transport->dgram_bind(vsk, addr);
}
