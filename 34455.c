do_authenticated(Authctxt *authctxt)
{
	setproctitle("%s", authctxt->pw->pw_name);

	/* setup the channel layer */
	/* XXX - streamlocal? */
	if (no_port_forwarding_flag ||
	    (options.allow_tcp_forwarding & FORWARD_LOCAL) == 0)
		channel_disable_adm_local_opens();
	else
		channel_permit_all_opens();

	auth_debug_send();

	if (compat20)
		do_authenticated2(authctxt);
	else
		do_authenticated1(authctxt);

	do_cleanup(authctxt);
}
