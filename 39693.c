vsock_bind(struct socket *sock, struct sockaddr *addr, int addr_len)
{
	int err;
	struct sock *sk;
	struct sockaddr_vm *vm_addr;

	sk = sock->sk;

	if (vsock_addr_cast(addr, addr_len, &vm_addr) != 0)
		return -EINVAL;

	lock_sock(sk);
	err = __vsock_bind(sk, vm_addr);
	release_sock(sk);

	return err;
}
