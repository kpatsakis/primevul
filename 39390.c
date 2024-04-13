static bool is_valid_mdb_entry(struct br_mdb_entry *entry)
{
	if (entry->ifindex == 0)
		return false;

	if (entry->addr.proto == htons(ETH_P_IP)) {
		if (!ipv4_is_multicast(entry->addr.u.ip4))
			return false;
		if (ipv4_is_local_multicast(entry->addr.u.ip4))
			return false;
#if IS_ENABLED(CONFIG_IPV6)
	} else if (entry->addr.proto == htons(ETH_P_IPV6)) {
		if (!ipv6_is_transient_multicast(&entry->addr.u.ip6))
			return false;
#endif
	} else
		return false;
	if (entry->state != MDB_PERMANENT && entry->state != MDB_TEMPORARY)
		return false;

	return true;
}
