ebt_dev_check(const char *entry, const struct net_device *device)
{
	int i = 0;
	const char *devname;

	if (*entry == '\0')
		return 0;
	if (!device)
		return 1;
	devname = device->name;
	/* 1 is the wildcard token */
	while (entry[i] != '\0' && entry[i] != 1 && entry[i] == devname[i])
		i++;
	return (devname[i] != entry[i] && entry[i] != 1);
}
