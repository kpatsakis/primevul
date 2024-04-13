static inline int qeth_create_skb_frag(struct qeth_qdio_buffer *qethbuffer,
		struct qdio_buffer_element *element,
		struct sk_buff **pskb, int offset, int *pfrag, int data_len)
{
	struct page *page = virt_to_page(element->addr);
	if (*pskb == NULL) {
		if (qethbuffer->rx_skb) {
			/* only if qeth_card.options.cq == QETH_CQ_ENABLED */
			*pskb = qethbuffer->rx_skb;
			qethbuffer->rx_skb = NULL;
		} else {
			*pskb = dev_alloc_skb(QETH_RX_PULL_LEN + ETH_HLEN);
			if (!(*pskb))
				return -ENOMEM;
		}

		skb_reserve(*pskb, ETH_HLEN);
		if (data_len <= QETH_RX_PULL_LEN) {
			memcpy(skb_put(*pskb, data_len), element->addr + offset,
				data_len);
		} else {
			get_page(page);
			memcpy(skb_put(*pskb, QETH_RX_PULL_LEN),
			       element->addr + offset, QETH_RX_PULL_LEN);
			skb_fill_page_desc(*pskb, *pfrag, page,
				offset + QETH_RX_PULL_LEN,
				data_len - QETH_RX_PULL_LEN);
			(*pskb)->data_len += data_len - QETH_RX_PULL_LEN;
			(*pskb)->len      += data_len - QETH_RX_PULL_LEN;
			(*pskb)->truesize += data_len - QETH_RX_PULL_LEN;
			(*pfrag)++;
		}
	} else {
		get_page(page);
		skb_fill_page_desc(*pskb, *pfrag, page, offset, data_len);
		(*pskb)->data_len += data_len;
		(*pskb)->len      += data_len;
		(*pskb)->truesize += data_len;
		(*pfrag)++;
	}


	return 0;
}
