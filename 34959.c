static void print_help(void)
{
	printf(_("Usage: %s [OPTION]...\n"), program_name);

	puts(_("psensor-server is an HTTP server for monitoring hardware "
	       "sensors remotely."));

	puts("");
	puts("Options:");
	puts(_("  -h, --help		display this help and exit\n"
	       "  -v, --version		display version information and exit"));

	puts("");
	puts(_("  -p,--port=PORT	webserver port\n"
	       "  -w,--wdir=DIR		directory containing webserver pages"));

	puts("");
	puts(_("  -d, --debug=LEVEL     "
	       "set the debug level, integer between 0 and 3"));
	puts(_("  -l, --log-file=PATH   set the log file to PATH"));
	puts(_("  --sensor-log-file=PATH set the sensor log file to PATH"));
	puts(_("  --sensor-log-interval=S "
	       "set the sensor log interval to S (seconds)"));

	puts("");
	printf(_("Report bugs to: %s\n"), PACKAGE_BUGREPORT);
	puts("");
	printf(_("%s home page: <%s>\n"), PACKAGE_NAME, PACKAGE_URL);
}
