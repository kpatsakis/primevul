smp_fetch_url32_src(struct proxy *px, struct session *l4, void *l7, unsigned int opt,
                     const struct arg *args, struct sample *smp, const char *kw)
{
	struct chunk *temp;
	struct connection *cli_conn = objt_conn(l4->si[0].end);

	if (!smp_fetch_url32(px, l4, l7, opt, args, smp, kw))
		return 0;

	temp = get_trash_chunk();
	memcpy(temp->str + temp->len, &smp->data.uint, sizeof(smp->data.uint));
	temp->len += sizeof(smp->data.uint);

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

	smp->data.str = *temp;
	smp->type = SMP_T_BIN;
	return 1;
}
