smp_fetch_url32_src(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	struct chunk *temp;
	struct connection *cli_conn = objt_conn(smp->sess->origin);

	if (!cli_conn)
		return 0;

	if (!smp_fetch_url32(args, smp, kw, private))
		return 0;

	temp = get_trash_chunk();
	*(unsigned int *)temp->str = htonl(smp->data.u.sint);
	temp->len += sizeof(unsigned int);

	switch (cli_conn->addr.from.ss_family) {
	case AF_INET:
		memcpy(temp->str + temp->len, &((struct sockaddr_in *)&cli_conn->addr.from)->sin_addr, 4);
		temp->len += 4;
		break;
	case AF_INET6:
		memcpy(temp->str + temp->len, &((struct sockaddr_in6 *)&cli_conn->addr.from)->sin6_addr, 16);
		temp->len += 16;
		break;
	default:
		return 0;
	}

	smp->data.u.str = *temp;
	smp->data.type = SMP_T_BIN;
	return 1;
}
