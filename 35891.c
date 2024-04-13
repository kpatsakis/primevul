void sock_update_netprioidx(struct sock *sk)
{
	if (in_interrupt())
		return;

	sk->sk_cgrp_prioidx = task_netprioidx(current);
}
