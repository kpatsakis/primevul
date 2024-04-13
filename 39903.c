static void afiucv_swap_src_dest(struct sk_buff *skb)
{
	struct af_iucv_trans_hdr *trans_hdr =
				(struct af_iucv_trans_hdr *)skb->data;
	char tmpID[8];
	char tmpName[8];

	ASCEBC(trans_hdr->destUserID, sizeof(trans_hdr->destUserID));
	ASCEBC(trans_hdr->destAppName, sizeof(trans_hdr->destAppName));
	ASCEBC(trans_hdr->srcUserID, sizeof(trans_hdr->srcUserID));
	ASCEBC(trans_hdr->srcAppName, sizeof(trans_hdr->srcAppName));
	memcpy(tmpID, trans_hdr->srcUserID, 8);
	memcpy(tmpName, trans_hdr->srcAppName, 8);
	memcpy(trans_hdr->srcUserID, trans_hdr->destUserID, 8);
	memcpy(trans_hdr->srcAppName, trans_hdr->destAppName, 8);
	memcpy(trans_hdr->destUserID, tmpID, 8);
	memcpy(trans_hdr->destAppName, tmpName, 8);
	skb_push(skb, ETH_HLEN);
	memset(skb->data, 0, ETH_HLEN);
}
