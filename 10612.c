inbound_nickserv_login (server *serv)
{
	/* this could grow ugly, but let's hope there won't be new NickServ types */
	switch (serv->loginmethod)
	{
		case LOGIN_MSG_NICKSERV:
		case LOGIN_NICKSERV:
		case LOGIN_CHALLENGEAUTH:
#if 0
		case LOGIN_NS:
		case LOGIN_MSG_NS:
		case LOGIN_AUTH:
#endif
			return 1;
		default:
			return 0;
	}
}