static ssize_t portio_porttype_show(struct uio_port *port, char *buf)
{
	const char *porttypes[] = {"none", "x86", "gpio", "other"};

	if ((port->porttype < 0) || (port->porttype > UIO_PORT_OTHER))
		return -EINVAL;

	return sprintf(buf, "port_%s\n", porttypes[port->porttype]);
}
