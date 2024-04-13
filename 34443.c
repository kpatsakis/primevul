int dtls1_send_finished(SSL *s, int a, int b, const char *sender, int slen)
	{
	unsigned char *p,*d;
	int i;
	unsigned long l;

	if (s->state == a)
		{
		d=(unsigned char *)s->init_buf->data;
		p= &(d[DTLS1_HM_HEADER_LENGTH]);

		i=s->method->ssl3_enc->final_finish_mac(s,
			sender,slen,s->s3->tmp.finish_md);
		s->s3->tmp.finish_md_len = i;
		memcpy(p, s->s3->tmp.finish_md, i);
		p+=i;
		l=i;

	/* Copy the finished so we can use it for
	 * renegotiation checks
	 */
	if(s->type == SSL_ST_CONNECT)
		{
		OPENSSL_assert(i <= EVP_MAX_MD_SIZE);
		memcpy(s->s3->previous_client_finished, 
		       s->s3->tmp.finish_md, i);
		s->s3->previous_client_finished_len=i;
		}
	else
		{
		OPENSSL_assert(i <= EVP_MAX_MD_SIZE);
		memcpy(s->s3->previous_server_finished, 
		       s->s3->tmp.finish_md, i);
		s->s3->previous_server_finished_len=i;
		}

#ifdef OPENSSL_SYS_WIN16
		/* MSVC 1.5 does not clear the top bytes of the word unless
		 * I do this.
		 */
		l&=0xffff;
#endif

		d = dtls1_set_message_header(s, d, SSL3_MT_FINISHED, l, 0, l);
		s->init_num=(int)l+DTLS1_HM_HEADER_LENGTH;
		s->init_off=0;

		/* buffer the message to handle re-xmits */
		dtls1_buffer_message(s, 0);

		s->state=b;
		}

	/* SSL3_ST_SEND_xxxxxx_HELLO_B */
	return(dtls1_do_write(s,SSL3_RT_HANDSHAKE));
	}
