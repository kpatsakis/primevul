dtls1_get_message_header(unsigned char *data, struct hm_header_st *msg_hdr)
	{
	memset(msg_hdr, 0x00, sizeof(struct hm_header_st));
	msg_hdr->type = *(data++);
	n2l3(data, msg_hdr->msg_len);

	n2s(data, msg_hdr->seq);
	n2l3(data, msg_hdr->frag_off);
	n2l3(data, msg_hdr->frag_len);
	}
