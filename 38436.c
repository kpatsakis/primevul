int qeth_hdr_chk_and_bounce(struct sk_buff *skb, struct qeth_hdr **hdr, int len)
{
	int hroom, inpage, rest;

	if (((unsigned long)skb->data & PAGE_MASK) !=
	    (((unsigned long)skb->data + len - 1) & PAGE_MASK)) {
		hroom = skb_headroom(skb);
		inpage = PAGE_SIZE - ((unsigned long) skb->data % PAGE_SIZE);
		rest = len - inpage;
		if (rest > hroom)
			return 1;
		memmove(skb->data - rest, skb->data, skb->len - skb->data_len);
		skb->data -= rest;
		skb->tail -= rest;
		*hdr = (struct qeth_hdr *)skb->data;
		QETH_DBF_MESSAGE(2, "skb bounce len: %d rest: %d\n", len, rest);
	}
	return 0;
}
