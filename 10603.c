inbound_identified (server *serv)	/* 'MODE +e MYSELF' on freenode */
{
	if (serv->joindelay_tag)
	{
		/* stop waiting, just auto JOIN now */
		fe_timeout_remove (serv->joindelay_tag);
		serv->joindelay_tag = 0;
		check_autojoin_channels (serv);
	}
}