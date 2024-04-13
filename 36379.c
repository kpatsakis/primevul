int rose_validate_nr(struct sock *sk, unsigned short nr)
{
	struct rose_sock *rose = rose_sk(sk);
	unsigned short vc = rose->va;

	while (vc != rose->vs) {
		if (nr == vc) return 1;
		vc = (vc + 1) % ROSE_MODULUS;
	}

	return nr == rose->vs;
}
