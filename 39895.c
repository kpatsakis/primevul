static int afiucv_hs_send(struct iucv_message *imsg, struct sock *sock,
		   struct sk_buff *skb, u8 flags)
{
	struct iucv_sock *iucv = iucv_sk(sock);
	struct af_iucv_trans_hdr *phs_hdr;
	struct sk_buff *nskb;
	int err, confirm_recv = 0;

	memset(skb->head, 0, ETH_HLEN);
	phs_hdr = (struct af_iucv_trans_hdr *)skb_push(skb,
					sizeof(struct af_iucv_trans_hdr));
	skb_reset_mac_header(skb);
	skb_reset_network_header(skb);
	skb_push(skb, ETH_HLEN);
	skb_reset_mac_header(skb);
	memset(phs_hdr, 0, sizeof(struct af_iucv_trans_hdr));

	phs_hdr->magic = ETH_P_AF_IUCV;
	phs_hdr->version = 1;
	phs_hdr->flags = flags;
	if (flags == AF_IUCV_FLAG_SYN)
		phs_hdr->window = iucv->msglimit;
	else if ((flags == AF_IUCV_FLAG_WIN) || !flags) {
		confirm_recv = atomic_read(&iucv->msg_recv);
		phs_hdr->window = confirm_recv;
		if (confirm_recv)
			phs_hdr->flags = phs_hdr->flags | AF_IUCV_FLAG_WIN;
	}
	memcpy(phs_hdr->destUserID, iucv->dst_user_id, 8);
	memcpy(phs_hdr->destAppName, iucv->dst_name, 8);
	memcpy(phs_hdr->srcUserID, iucv->src_user_id, 8);
	memcpy(phs_hdr->srcAppName, iucv->src_name, 8);
	ASCEBC(phs_hdr->destUserID, sizeof(phs_hdr->destUserID));
	ASCEBC(phs_hdr->destAppName, sizeof(phs_hdr->destAppName));
	ASCEBC(phs_hdr->srcUserID, sizeof(phs_hdr->srcUserID));
	ASCEBC(phs_hdr->srcAppName, sizeof(phs_hdr->srcAppName));
	if (imsg)
		memcpy(&phs_hdr->iucv_hdr, imsg, sizeof(struct iucv_message));

	skb->dev = iucv->hs_dev;
	if (!skb->dev)
		return -ENODEV;
	if (!(skb->dev->flags & IFF_UP) || !netif_carrier_ok(skb->dev))
		return -ENETDOWN;
	if (skb->len > skb->dev->mtu) {
		if (sock->sk_type == SOCK_SEQPACKET)
			return -EMSGSIZE;
		else
			skb_trim(skb, skb->dev->mtu);
	}
	skb->protocol = ETH_P_AF_IUCV;
	nskb = skb_clone(skb, GFP_ATOMIC);
	if (!nskb)
		return -ENOMEM;
	skb_queue_tail(&iucv->send_skb_q, nskb);
	err = dev_queue_xmit(skb);
	if (net_xmit_eval(err)) {
		skb_unlink(nskb, &iucv->send_skb_q);
		kfree_skb(nskb);
	} else {
		atomic_sub(confirm_recv, &iucv->msg_recv);
		WARN_ON(atomic_read(&iucv->msg_recv) < 0);
	}
	return net_xmit_eval(err);
}
