int qeth_get_elements_for_frags(struct sk_buff *skb)
{
	int cnt, length, e, elements = 0;
	struct skb_frag_struct *frag;
	char *data;

	for (cnt = 0; cnt < skb_shinfo(skb)->nr_frags; cnt++) {
		frag = &skb_shinfo(skb)->frags[cnt];
		data = (char *)page_to_phys(skb_frag_page(frag)) +
			frag->page_offset;
		length = frag->size;
		e = PFN_UP((unsigned long)data + length - 1) -
			PFN_DOWN((unsigned long)data);
		elements += e;
	}
	return elements;
}
