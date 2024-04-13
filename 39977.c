static int caif_sktrecv_cb(struct cflayer *layr, struct cfpkt *pkt)
{
	struct caifsock *cf_sk;
	struct sk_buff *skb;

	cf_sk = container_of(layr, struct caifsock, layer);
	skb = cfpkt_tonative(pkt);

	if (unlikely(cf_sk->sk.sk_state != CAIF_CONNECTED)) {
		kfree_skb(skb);
		return 0;
	}
	caif_queue_rcv_skb(&cf_sk->sk, skb);
	return 0;
}
