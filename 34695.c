static char *allocate_tempopt_if_needed(
		const struct dhcp_optflag *optflag,
		char *buffer,
		int *length_p)
{
	char *allocated = NULL;
	if ((optflag->flags & OPTION_TYPE_MASK) == OPTION_BIN) {
		const char *end;
		allocated = xstrdup(buffer); /* more than enough */
		end = hex2bin(allocated, buffer, 255);
		if (errno)
			bb_error_msg_and_die("malformed hex string '%s'", buffer);
		*length_p = end - allocated;
	}
	return allocated;
}
