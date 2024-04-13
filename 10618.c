inbound_cap_nak (server *serv, const message_tags_data *tags_data)
{
	serv->sent_capend = TRUE;
	tcp_send_len (serv, "CAP END\r\n", 9);
}