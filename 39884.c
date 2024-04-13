static inline struct l2tp_ip6_sock *l2tp_ip6_sk(const struct sock *sk)
{
	return (struct l2tp_ip6_sock *)sk;
}
