static void c_write(Ssh ssh, const char *buf, int len)
{
    if (flags & FLAG_STDERR)
	c_write_stderr(1, buf, len);
    else
	from_backend(ssh->frontend, 1, buf, len);
}
