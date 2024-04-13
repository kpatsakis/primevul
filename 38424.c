int qeth_get_elements_no(struct qeth_card *card,
		     struct sk_buff *skb, int elems)
{
	int dlen = skb->len - skb->data_len;
	int elements_needed = PFN_UP((unsigned long)skb->data + dlen - 1) -
		PFN_DOWN((unsigned long)skb->data);

	elements_needed += qeth_get_elements_for_frags(skb);

	if ((elements_needed + elems) > QETH_MAX_BUFFER_ELEMENTS(card)) {
		QETH_DBF_MESSAGE(2, "Invalid size of IP packet "
			"(Number=%d / Length=%d). Discarded.\n",
			(elements_needed+elems), skb->len);
		return 0;
	}
	return elements_needed;
}
