static void c_write_stderr(int trusted, const char *buf, int len)
{
    int i;
    for (i = 0; i < len; i++)
	if (buf[i] != '\r' && (trusted || buf[i] == '\n' || (buf[i] & 0x60)))
	    fputc(buf[i], stderr);
}
