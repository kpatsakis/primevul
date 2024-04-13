lspci_send(const char *output)
{
	STREAM s;
	size_t len;

	len = strlen(output);
	s = channel_init(lspci_channel, len);
	out_uint8p(s, output, len) s_mark_end(s);
	channel_send(s, lspci_channel);
}
