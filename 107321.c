void usage()
{
	printf("\n"
		"  %s - (C) 2007,2008 Andrea Bittau\n"
		"  http://www.aircrack-ng.org\n"
		"\n"
		"  Usage: buddy-ng <options>\n"
		"\n"
		"  Options:\n"
		"\n"
		"       -h        : This help screen\n"
		"       -p        : Don't drop privileges\n"
		"\n",
		getVersion("Buddy-ng", _MAJ, _MIN, _SUB_MIN, _REVISION, _BETA, _RC));

	exit(1);
}
