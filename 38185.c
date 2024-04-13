static ssize_t portio_name_show(struct uio_port *port, char *buf)
{
	if (unlikely(!port->name))
		port->name = "";

	return sprintf(buf, "%s\n", port->name);
}
