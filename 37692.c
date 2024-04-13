static void _ldm_printk (const char *level, const char *function,
			 const char *fmt, ...)
{
	static char buf[128];
	va_list args;

	va_start (args, fmt);
	vsnprintf (buf, sizeof (buf), fmt, args);
	va_end (args);

	printk ("%s%s(): %s\n", level, function, buf);
}
