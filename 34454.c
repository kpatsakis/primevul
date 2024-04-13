display_loginmsg(void)
{
	if (buffer_len(&loginmsg) > 0) {
		buffer_append(&loginmsg, "\0", 1);
		printf("%s", (char *)buffer_ptr(&loginmsg));
		buffer_clear(&loginmsg);
	}
}
