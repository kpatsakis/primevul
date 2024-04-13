main(int argc, char* argv[])
{
    extern int optind;
    extern char *optarg;
    int c, status;

    while ((c = getopt(argc, argv, "c:r:")) != -1)
	    switch (c) {
	    case 'c':		/* compression scheme */
		    if (!processCompressOptions(optarg))
			    usage();
		    break;
	    case 'r':		/* rows/strip */
		    rowsperstrip = atoi(optarg);
		    break;
	    case '?':
		    usage();
		    /*NOTREACHED*/
	    }
    if (argc - optind != 2)
	    usage();

    makegamtab(GIFGAMMA);
    filename = argv[optind];
    imagename = argv[optind+1];
    if ((infile = fopen(imagename, "rb")) != NULL) {
	int c;
	fclose(infile);
	printf("overwrite %s? ", imagename); fflush(stdout);
	c = getc(stdin);
	if (c != 'y' && c != 'Y')
	    return (1);
    }
    if ((infile = fopen(filename, "rb")) == NULL) {
	perror(filename);
	return (1);
    }
    status = convert();
    fclose(infile);
    return (status);
}
