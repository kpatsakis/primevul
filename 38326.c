ssize_t aac_get_serial_number(struct device *device, char *buf)
{
	return aac_show_serial_number(device, &aac_serial_number, buf);
}
