static int caif_release(struct socket *sock)
{
	struct sock *sk = sock->sk;
	struct caifsock *cf_sk = container_of(sk, struct caifsock, sk);

	if (!sk)
		return 0;

	set_tx_flow_off(cf_sk);

	/*
	 * Ensure that packets are not queued after this point in time.
	 * caif_queue_rcv_skb checks SOCK_DEAD holding the queue lock,
	 * this ensures no packets when sock is dead.
	 */
	spin_lock_bh(&sk->sk_receive_queue.lock);
	sock_set_flag(sk, SOCK_DEAD);
	spin_unlock_bh(&sk->sk_receive_queue.lock);
	sock->sk = NULL;

	WARN_ON(IS_ERR(cf_sk->debugfs_socket_dir));
	if (cf_sk->debugfs_socket_dir != NULL)
		debugfs_remove_recursive(cf_sk->debugfs_socket_dir);

	lock_sock(&(cf_sk->sk));
	sk->sk_state = CAIF_DISCONNECTED;
	sk->sk_shutdown = SHUTDOWN_MASK;

	caif_disconnect_client(sock_net(sk), &cf_sk->layer);
	cf_sk->sk.sk_socket->state = SS_DISCONNECTING;
	wake_up_interruptible_poll(sk_sleep(sk), POLLERR|POLLHUP);

	sock_orphan(sk);
	sk_stream_kill_queues(&cf_sk->sk);
	release_sock(sk);
	sock_put(sk);
	return 0;
}
