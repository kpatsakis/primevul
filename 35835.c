void sk_prot_clear_portaddr_nulls(struct sock *sk, int size)
{
	unsigned long nulls1, nulls2;

	nulls1 = offsetof(struct sock, __sk_common.skc_node.next);
	nulls2 = offsetof(struct sock, __sk_common.skc_portaddr_node.next);
	if (nulls1 > nulls2)
		swap(nulls1, nulls2);

	if (nulls1 != 0)
		memset((char *)sk, 0, nulls1);
	memset((char *)sk + nulls1 + sizeof(void *), 0,
	       nulls2 - nulls1 - sizeof(void *));
	memset((char *)sk + nulls2 + sizeof(void *), 0,
	       size - nulls2 - sizeof(void *));
}
