dtls1_retransmit_buffered_messages(SSL *s)
	{
	pqueue sent = s->d1->sent_messages;
	piterator iter;
	pitem *item;
	hm_fragment *frag;
	int found = 0;

	iter = pqueue_iterator(sent);

	for ( item = pqueue_next(&iter); item != NULL; item = pqueue_next(&iter))
		{
		frag = (hm_fragment *)item->data;
			if ( dtls1_retransmit_message(s,
				(unsigned short)dtls1_get_queue_priority(frag->msg_header.seq, frag->msg_header.is_ccs),
				0, &found) <= 0 && found)
			{
			fprintf(stderr, "dtls1_retransmit_message() failed\n");
			return -1;
			}
		}

	return 1;
	}
