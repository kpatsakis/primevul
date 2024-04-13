int ssl3_do_write(SSL *s, int type)
	{
	int ret;

	ret=ssl3_write_bytes(s,type,&s->init_buf->data[s->init_off],
	                     s->init_num);
	if (ret < 0) return(-1);
	if (type == SSL3_RT_HANDSHAKE)
		/* should not be done for 'Hello Request's, but in that case
		 * we'll ignore the result anyway */
		ssl3_finish_mac(s,(unsigned char *)&s->init_buf->data[s->init_off],ret);
	
	if (ret == s->init_num)
		{
		if (s->msg_callback)
			s->msg_callback(1, s->version, type, s->init_buf->data, (size_t)(s->init_off + s->init_num), s, s->msg_callback_arg);
		return(1);
		}
	s->init_off+=ret;
	s->init_num-=ret;
	return(0);
	}
