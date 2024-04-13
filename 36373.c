void rose_clear_queues(struct sock *sk)
{
	skb_queue_purge(&sk->sk_write_queue);
	skb_queue_purge(&rose_sk(sk)->ack_queue);
}
