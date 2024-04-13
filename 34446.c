dtls1_write_message_header(SSL *s, unsigned char *p)
	{
	struct hm_header_st *msg_hdr = &s->d1->w_msg_hdr;

	*p++ = msg_hdr->type;
	l2n3(msg_hdr->msg_len, p);

	s2n(msg_hdr->seq, p);
	l2n3(msg_hdr->frag_off, p);
	l2n3(msg_hdr->frag_len, p);

	return p;
	}
