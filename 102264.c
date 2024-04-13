configure_env(char **env, const char *prefix, const struct dhcp_message *dhcp,
 const struct if_options *ifo)
{
 unsigned int i;
 const uint8_t *p;
 int pl;
 struct in_addr addr;
 struct in_addr net;
 struct in_addr brd;
 char *val, *v;
 const struct dhcp_opt *opt;
 ssize_t len, e = 0;
 char **ep;
 char cidr[4];
 uint8_t overl = 0;

	get_option_uint8(&overl, dhcp, DHO_OPTIONSOVERLOADED);

 if (!env) {
 for (opt = dhcp_opts; opt->option; opt++) {
 if (!opt->var)
 continue;
 if (has_option_mask(ifo->nomask, opt->option))
 continue;
 if (get_option_raw(dhcp, opt->option))
				e++;
 }
 if (dhcp->yiaddr || dhcp->ciaddr)
			e += 5;
 if (*dhcp->bootfile && !(overl & 1))
			e++;
 if (*dhcp->servername && !(overl & 2))
			e++;
 return e;
 }

	ep = env;
 if (dhcp->yiaddr || dhcp->ciaddr) {
 /* Set some useful variables that we derive from the DHCP
		 * message but are not necessarily in the options */
		addr.s_addr = dhcp->yiaddr ? dhcp->yiaddr : dhcp->ciaddr;
		setvar(&ep, prefix, "ip_address", inet_ntoa(addr));
 if (get_option_addr(&net, dhcp, DHO_SUBNETMASK) == -1) {
			net.s_addr = get_netmask(addr.s_addr);
			setvar(&ep, prefix, "subnet_mask", inet_ntoa(net));
 }
		i = inet_ntocidr(net);
		snprintf(cidr, sizeof(cidr), "%d", inet_ntocidr(net));
		setvar(&ep, prefix, "subnet_cidr", cidr);
 if (get_option_addr(&brd, dhcp, DHO_BROADCAST) == -1) {
			brd.s_addr = addr.s_addr | ~net.s_addr;
			setvar(&ep, prefix, "broadcast_address", inet_ntoa(brd));
 }
		addr.s_addr = dhcp->yiaddr & net.s_addr;
		setvar(&ep, prefix, "network_number", inet_ntoa(addr));
 }

 if (*dhcp->bootfile && !(overl & 1))
		setvar(&ep, prefix, "filename", (const char *)dhcp->bootfile);
 if (*dhcp->servername && !(overl & 2))
		setvar(&ep, prefix, "server_name", (const char *)dhcp->servername);

 for (opt = dhcp_opts; opt->option; opt++) {
 if (!opt->var)
 continue;
 if (has_option_mask(ifo->nomask, opt->option))
 continue;
		val = NULL;
		p = get_option(dhcp, opt->option, &pl, NULL);
 if (!p)
 continue;
 /* We only want the FQDN name */
 if (opt->option == DHO_FQDN) {
			p += 3;
			pl -= 3;
 }
		len = print_option(NULL, 0, opt->type, pl, p);
 if (len < 0)
 return -1;
		e = strlen(prefix) + strlen(opt->var) + len + 4;
		v = val = *ep++ = xmalloc(e);
		v += snprintf(val, e, "%s_%s=", prefix, opt->var);
 if (len != 0)
			print_option(v, len, opt->type, pl, p);
 }

 return ep - env;
}
