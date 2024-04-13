long dtls1_get_message(SSL *s, int st1, int stn, int mt, long max, int *ok)
	{
	int i, al;
	struct hm_header_st *msg_hdr;
	unsigned char *p;
	unsigned long msg_len;

	/* s3->tmp is used to store messages that are unexpected, caused
	 * by the absence of an optional handshake message */
	if (s->s3->tmp.reuse_message)
		{
		s->s3->tmp.reuse_message=0;
		if ((mt >= 0) && (s->s3->tmp.message_type != mt))
			{
			al=SSL_AD_UNEXPECTED_MESSAGE;
			SSLerr(SSL_F_DTLS1_GET_MESSAGE,SSL_R_UNEXPECTED_MESSAGE);
			goto f_err;
			}
		*ok=1;
		s->init_msg = s->init_buf->data + DTLS1_HM_HEADER_LENGTH;
		s->init_num = (int)s->s3->tmp.message_size;
		return s->init_num;
		}

	msg_hdr = &s->d1->r_msg_hdr;
	memset(msg_hdr, 0x00, sizeof(struct hm_header_st));

again:
	i = dtls1_get_message_fragment(s, st1, stn, max, ok);
	if ( i == DTLS1_HM_BAD_FRAGMENT ||
		i == DTLS1_HM_FRAGMENT_RETRY)  /* bad fragment received */
		goto again;
	else if ( i <= 0 && !*ok)
		return i;

	p = (unsigned char *)s->init_buf->data;
	msg_len = msg_hdr->msg_len;

	/* reconstruct message header */
	*(p++) = msg_hdr->type;
	l2n3(msg_len,p);
	s2n (msg_hdr->seq,p);
	l2n3(0,p);
	l2n3(msg_len,p);
	if (s->version != DTLS1_BAD_VER) {
		p       -= DTLS1_HM_HEADER_LENGTH;
		msg_len += DTLS1_HM_HEADER_LENGTH;
	}

	ssl3_finish_mac(s, p, msg_len);
	if (s->msg_callback)
		s->msg_callback(0, s->version, SSL3_RT_HANDSHAKE,
			p, msg_len,
			s, s->msg_callback_arg);

	memset(msg_hdr, 0x00, sizeof(struct hm_header_st));

	/* Don't change sequence numbers while listening */
	if (!s->d1->listen)
		s->d1->handshake_read_seq++;

	s->init_msg = s->init_buf->data + DTLS1_HM_HEADER_LENGTH;
	return s->init_num;

f_err:
	ssl3_send_alert(s,SSL3_AL_FATAL,al);
	*ok = 0;
	return -1;
	}
