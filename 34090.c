CheckUserParameters(int argc, char **argv, char **envp)
{
    enum BadCode bad = NotBad;
    int i = 0, j;
    char *a, *e = NULL;

#if CHECK_EUID
    if (geteuid() == 0 && getuid() != geteuid())
#endif
    {
	/* Check each argv[] */
	for (i = 1; i < argc; i++) {
	    if (strcmp(argv[i], "-fp") == 0)
	    {
		i++; /* continue with next argument. skip the length check */
		if (i >= argc)
		    break;
	    } else
	    {
		if (strlen(argv[i]) > MAX_ARG_LENGTH) {
		    bad = ArgTooLong;
		    break;
		}
	    }
	    a = argv[i];
	    while (*a) {
		if (checkPrintable(*a) == 0) {
		    bad = UnprintableArg;
		    break;
		}
		a++;
	    }
	    if (bad)
		break;
	}
	if (!bad) {
	    /* Check each envp[] */
	    for (i = 0; envp[i]; i++) {

		/* Check for bad environment variables and values */
#if REMOVE_ENV_LD
		while (envp[i] && (strncmp(envp[i], "LD", 2) == 0)) {
		    for (j = i; envp[j]; j++) {
			envp[j] = envp[j+1];
		    }
		}
#endif   
		if (envp[i] && (strlen(envp[i]) > MAX_ENV_LENGTH)) {
#if REMOVE_LONG_ENV
		    for (j = i; envp[j]; j++) {
			envp[j] = envp[j+1];
		    }
		    i--;
#else
		    char *eq;
		    int len;

		    eq = strchr(envp[i], '=');
		    if (!eq)
			continue;
		    len = eq - envp[i];
		    e = malloc(len + 1);
		    if (!e) {
			bad = InternalError;
			break;
		    }
		    strncpy(e, envp[i], len);
		    e[len] = 0;
		    if (len >= 4 &&
			(strcmp(e + len - 4, "PATH") == 0 ||
			 strcmp(e, "TERMCAP") == 0)) {
			if (strlen(envp[i]) > MAX_ENV_PATH_LENGTH) {
			    bad = EnvTooLong;
			    break;
			} else {
			    free(e);
			}
		    } else {
			bad = EnvTooLong;
			break;
		    }
#endif
		}
	    }
	}
#if NO_OUTPUT_PIPES
	if (!bad) {
	    struct stat buf;

	    if (fstat(fileno(stdout), &buf) == 0 && S_ISFIFO(buf.st_mode))
		bad = OutputIsPipe;
	    if (fstat(fileno(stderr), &buf) == 0 && S_ISFIFO(buf.st_mode))
		bad = OutputIsPipe;
	}
#endif
    }
    switch (bad) {
    case NotBad:
	return;
    case UnsafeArg:
	ErrorF("Command line argument number %d is unsafe\n", i);
	break;
    case ArgTooLong:
	ErrorF("Command line argument number %d is too long\n", i);
	break;
    case UnprintableArg:
	ErrorF("Command line argument number %d contains unprintable"
		" characters\n", i);
	break;
    case EnvTooLong:
	ErrorF("Environment variable `%s' is too long\n", e);
	break;
    case OutputIsPipe:
	ErrorF("Stdout and/or stderr is a pipe\n");
	break;
    case InternalError:
	ErrorF("Internal Error\n");
	break;
    default:
	ErrorF("Unknown error\n");
	break;
    }
    FatalError("X server aborted because of unsafe environment\n");
}
