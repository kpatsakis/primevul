int dtls1_send_change_cipher_spec(SSL *s, int a, int b)
	{ 
	unsigned char *p;

	if (s->state == a)
		{
		p=(unsigned char *)s->init_buf->data;
		*p++=SSL3_MT_CCS;
		s->d1->handshake_write_seq = s->d1->next_handshake_write_seq;
		s->init_num=DTLS1_CCS_HEADER_LENGTH;

		if (s->version == DTLS1_BAD_VER) {
			s->d1->next_handshake_write_seq++;
			s2n(s->d1->handshake_write_seq,p);
			s->init_num+=2;
		}

		s->init_off=0;

		dtls1_set_message_header_int(s, SSL3_MT_CCS, 0, 
			s->d1->handshake_write_seq, 0, 0);

		/* buffer the message to handle re-xmits */
		dtls1_buffer_message(s, 1);

		s->state=b;
		}

	/* SSL3_ST_CW_CHANGE_B */
	return(dtls1_do_write(s,SSL3_RT_CHANGE_CIPHER_SPEC));
	}
