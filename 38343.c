static inline void __qeth_fill_buffer(struct sk_buff *skb,
	struct qdio_buffer *buffer, int is_tso, int *next_element_to_fill,
	int offset)
{
	int length = skb->len - skb->data_len;
	int length_here;
	int element;
	char *data;
	int first_lap, cnt;
	struct skb_frag_struct *frag;

	element = *next_element_to_fill;
	data = skb->data;
	first_lap = (is_tso == 0 ? 1 : 0);

	if (offset >= 0) {
		data = skb->data + offset;
		length -= offset;
		first_lap = 0;
	}

	while (length > 0) {
		/* length_here is the remaining amount of data in this page */
		length_here = PAGE_SIZE - ((unsigned long) data % PAGE_SIZE);
		if (length < length_here)
			length_here = length;

		buffer->element[element].addr = data;
		buffer->element[element].length = length_here;
		length -= length_here;
		if (!length) {
			if (first_lap)
				if (skb_shinfo(skb)->nr_frags)
					buffer->element[element].eflags =
						SBAL_EFLAGS_FIRST_FRAG;
				else
					buffer->element[element].eflags = 0;
			else
				buffer->element[element].eflags =
				    SBAL_EFLAGS_MIDDLE_FRAG;
		} else {
			if (first_lap)
				buffer->element[element].eflags =
				    SBAL_EFLAGS_FIRST_FRAG;
			else
				buffer->element[element].eflags =
				    SBAL_EFLAGS_MIDDLE_FRAG;
		}
		data += length_here;
		element++;
		first_lap = 0;
	}

	for (cnt = 0; cnt < skb_shinfo(skb)->nr_frags; cnt++) {
		frag = &skb_shinfo(skb)->frags[cnt];
		data = (char *)page_to_phys(skb_frag_page(frag)) +
			frag->page_offset;
		length = frag->size;
		while (length > 0) {
			length_here = PAGE_SIZE -
				((unsigned long) data % PAGE_SIZE);
			if (length < length_here)
				length_here = length;

			buffer->element[element].addr = data;
			buffer->element[element].length = length_here;
			buffer->element[element].eflags =
				SBAL_EFLAGS_MIDDLE_FRAG;
			length -= length_here;
			data += length_here;
			element++;
		}
	}

	if (buffer->element[element - 1].eflags)
		buffer->element[element - 1].eflags = SBAL_EFLAGS_LAST_FRAG;
	*next_element_to_fill = element;
}
