void parse_wait_interval(int argc, char *argv[], struct timeval *wait_interval)
{
	const char *arg;
	unsigned int usec;
	int ret;

	if (optarg)
		arg = optarg;
	else if (xs_has_arg(argc, argv))
		arg = argv[optind++];
	else
		xtables_error(PARAMETER_PROBLEM, "wait interval value required");

	ret = sscanf(arg, "%u", &usec);
	if (ret == 1) {
		if (usec > 999999)
			xtables_error(PARAMETER_PROBLEM,
				      "too long usec wait %u > 999999 usec",
				      usec);

		wait_interval->tv_sec = 0;
		wait_interval->tv_usec = usec;
		return;
	}
	xtables_error(PARAMETER_PROBLEM, "wait interval not numeric");
}
