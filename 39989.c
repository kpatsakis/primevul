static int transmit_skb(struct sk_buff *skb, struct caifsock *cf_sk,
			int noblock, long timeo)
{
	struct cfpkt *pkt;

	pkt = cfpkt_fromnative(CAIF_DIR_OUT, skb);
	memset(skb->cb, 0, sizeof(struct caif_payload_info));
	cfpkt_set_prio(pkt, cf_sk->sk.sk_priority);

	if (cf_sk->layer.dn == NULL) {
		kfree_skb(skb);
		return -EINVAL;
	}

	return cf_sk->layer.dn->transmit(cf_sk->layer.dn, pkt);
}
