smp_fetch_hdr_ip(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	int ret;

	while ((ret = smp_fetch_hdr(args, smp, kw, private)) > 0) {
		if (url2ipv4((char *)smp->data.u.str.str, &smp->data.u.ipv4)) {
			smp->data.type = SMP_T_IPV4;
			break;
		} else {
			struct chunk *temp = get_trash_chunk();
			if (smp->data.u.str.len < temp->size - 1) {
				memcpy(temp->str, smp->data.u.str.str, smp->data.u.str.len);
				temp->str[smp->data.u.str.len] = '\0';
				if (inet_pton(AF_INET6, temp->str, &smp->data.u.ipv6)) {
					smp->data.type = SMP_T_IPV6;
					break;
				}
			}
		}

		/* if the header doesn't match an IP address, fetch next one */
		if (!(smp->flags & SMP_F_NOT_LAST))
			return 0;
	}
	return ret;
}
