void sk_reset_txq(struct sock *sk)
{
	sk_tx_queue_clear(sk);
}
