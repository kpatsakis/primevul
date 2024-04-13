do_dns (session *sess, char *nick, char *host,
		const message_tags_data *tags_data)
{
	GResolver *res = g_resolver_get_default ();
	GInetAddress *addr;
	char *po;

	po = strrchr (host, '@');
	if (po)
		host = po + 1;

	if (nick)
		EMIT_SIGNAL_TIMESTAMP (XP_TE_RESOLVINGUSER, sess, nick, host, NULL, NULL, 0,
								tags_data->timestamp);

	PrintTextf (sess, _("Looking up %s..."), host);

	addr = g_inet_address_new_from_string (host);
	if (addr)
		g_resolver_lookup_by_address_async (res, addr, NULL, dns_addr_callback, sess);
	else
		g_resolver_lookup_by_name_async (res, host, NULL, dns_name_callback, sess);
}