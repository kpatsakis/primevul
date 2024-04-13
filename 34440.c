dtls1_retransmit_message(SSL *s, unsigned short seq, unsigned long frag_off,
	int *found)
	{
	int ret;
	/* XDTLS: for now assuming that read/writes are blocking */
	pitem *item;
	hm_fragment *frag ;
	unsigned long header_length;
	unsigned char seq64be[8];
	struct dtls1_retransmit_state saved_state;
	unsigned char save_write_sequence[8];

	/*
	  OPENSSL_assert(s->init_num == 0);
	  OPENSSL_assert(s->init_off == 0);
	 */

	/* XDTLS:  the requested message ought to be found, otherwise error */
	memset(seq64be,0,sizeof(seq64be));
	seq64be[6] = (unsigned char)(seq>>8);
	seq64be[7] = (unsigned char)seq;

	item = pqueue_find(s->d1->sent_messages, seq64be);
	if ( item == NULL)
		{
		fprintf(stderr, "retransmit:  message %d non-existant\n", seq);
		*found = 0;
		return 0;
		}

	*found = 1;
	frag = (hm_fragment *)item->data;

	if ( frag->msg_header.is_ccs)
		header_length = DTLS1_CCS_HEADER_LENGTH;
	else
		header_length = DTLS1_HM_HEADER_LENGTH;

	memcpy(s->init_buf->data, frag->fragment, 
		frag->msg_header.msg_len + header_length);
		s->init_num = frag->msg_header.msg_len + header_length;

	dtls1_set_message_header_int(s, frag->msg_header.type, 
		frag->msg_header.msg_len, frag->msg_header.seq, 0, 
		frag->msg_header.frag_len);

	/* save current state */
	saved_state.enc_write_ctx = s->enc_write_ctx;
	saved_state.write_hash = s->write_hash;
	saved_state.compress = s->compress;
	saved_state.session = s->session;
	saved_state.epoch = s->d1->w_epoch;
	saved_state.epoch = s->d1->w_epoch;
	
	s->d1->retransmitting = 1;
	
	/* restore state in which the message was originally sent */
	s->enc_write_ctx = frag->msg_header.saved_retransmit_state.enc_write_ctx;
	s->write_hash = frag->msg_header.saved_retransmit_state.write_hash;
	s->compress = frag->msg_header.saved_retransmit_state.compress;
	s->session = frag->msg_header.saved_retransmit_state.session;
	s->d1->w_epoch = frag->msg_header.saved_retransmit_state.epoch;
	
	if (frag->msg_header.saved_retransmit_state.epoch == saved_state.epoch - 1)
	{
		memcpy(save_write_sequence, s->s3->write_sequence, sizeof(s->s3->write_sequence));
		memcpy(s->s3->write_sequence, s->d1->last_write_sequence, sizeof(s->s3->write_sequence));
	}
	
	ret = dtls1_do_write(s, frag->msg_header.is_ccs ? 
						 SSL3_RT_CHANGE_CIPHER_SPEC : SSL3_RT_HANDSHAKE);
	
	/* restore current state */
	s->enc_write_ctx = saved_state.enc_write_ctx;
	s->write_hash = saved_state.write_hash;
	s->compress = saved_state.compress;
	s->session = saved_state.session;
	s->d1->w_epoch = saved_state.epoch;
	
	if (frag->msg_header.saved_retransmit_state.epoch == saved_state.epoch - 1)
	{
		memcpy(s->d1->last_write_sequence, s->s3->write_sequence, sizeof(s->s3->write_sequence));
		memcpy(s->s3->write_sequence, save_write_sequence, sizeof(s->s3->write_sequence));
	}

	s->d1->retransmitting = 0;

	(void)BIO_flush(SSL_get_wbio(s));
	return ret;
	}
