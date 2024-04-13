static ssize_t portio_start_show(struct uio_port *port, char *buf)
{
	return sprintf(buf, "0x%lx\n", port->start);
}
