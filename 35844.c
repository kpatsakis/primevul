static void sk_update_clone(const struct sock *sk, struct sock *newsk)
{
	if (mem_cgroup_sockets_enabled && sk->sk_cgrp)
		sock_update_memcg(newsk);
}
