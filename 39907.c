void iucv_accept_unlink(struct sock *sk)
{
	unsigned long flags;
	struct iucv_sock *par = iucv_sk(iucv_sk(sk)->parent);

	spin_lock_irqsave(&par->accept_q_lock, flags);
	list_del_init(&iucv_sk(sk)->accept_q);
	spin_unlock_irqrestore(&par->accept_q_lock, flags);
	sk_acceptq_removed(iucv_sk(sk)->parent);
	iucv_sk(sk)->parent = NULL;
	sock_put(sk);
}
