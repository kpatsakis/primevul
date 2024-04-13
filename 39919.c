static struct sock *iucv_sock_alloc(struct socket *sock, int proto, gfp_t prio)
{
	struct sock *sk;
	struct iucv_sock *iucv;

	sk = sk_alloc(&init_net, PF_IUCV, prio, &iucv_proto);
	if (!sk)
		return NULL;
	iucv = iucv_sk(sk);

	sock_init_data(sock, sk);
	INIT_LIST_HEAD(&iucv->accept_q);
	spin_lock_init(&iucv->accept_q_lock);
	skb_queue_head_init(&iucv->send_skb_q);
	INIT_LIST_HEAD(&iucv->message_q.list);
	spin_lock_init(&iucv->message_q.lock);
	skb_queue_head_init(&iucv->backlog_skb_q);
	iucv->send_tag = 0;
	atomic_set(&iucv->pendings, 0);
	iucv->flags = 0;
	iucv->msglimit = 0;
	atomic_set(&iucv->msg_sent, 0);
	atomic_set(&iucv->msg_recv, 0);
	iucv->path = NULL;
	iucv->sk_txnotify = afiucv_hs_callback_txnotify;
	memset(&iucv->src_user_id , 0, 32);
	if (pr_iucv)
		iucv->transport = AF_IUCV_TRANS_IUCV;
	else
		iucv->transport = AF_IUCV_TRANS_HIPER;

	sk->sk_destruct = iucv_sock_destruct;
	sk->sk_sndtimeo = IUCV_CONN_TIMEOUT;
	sk->sk_allocation = GFP_DMA;

	sock_reset_flag(sk, SOCK_ZAPPED);

	sk->sk_protocol = proto;
	sk->sk_state	= IUCV_OPEN;

	iucv_sock_link(&iucv_sk_list, sk);
	return sk;
}
