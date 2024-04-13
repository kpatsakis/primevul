dtls1_set_message_header_int(SSL *s, unsigned char mt,
			    unsigned long len, unsigned short seq_num, unsigned long frag_off,
			    unsigned long frag_len)
	{
	struct hm_header_st *msg_hdr = &s->d1->w_msg_hdr;

	msg_hdr->type = mt;
	msg_hdr->msg_len = len;
	msg_hdr->seq = seq_num;
	msg_hdr->frag_off = frag_off;
	msg_hdr->frag_len = frag_len;
	}
