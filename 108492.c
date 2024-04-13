main(int argc, char **argv)
{
	const char *safepath = SAFE_PATH;
	const char *confpath = NULL;
	char *shargv[] = { NULL, NULL };
	char *sh;
	const char *cmd;
	char cmdline[LINE_MAX];
	char myname[_PW_NAME_LEN + 1];
	struct passwd *original_pw, *target_pw;
	struct rule *rule;
	uid_t uid;
	uid_t target = 0;
	gid_t groups[NGROUPS_MAX + 1];
	int ngroups;
	int i, ch;
	int sflag = 0;
	int nflag = 0;
	char cwdpath[PATH_MAX];
	const char *cwd;
	char *login_style = NULL;
	char **envp;

	#ifndef linux
	setprogname("doas");
	#endif

	#ifndef linux
	closefrom(STDERR_FILENO + 1);
	#endif

	uid = getuid();

	while ((ch = getopt(argc, argv, "a:C:nsu:")) != -1) {
/*	while ((ch = getopt(argc, argv, "a:C:Lnsu:")) != -1) { */
		switch (ch) {
		case 'a':
			login_style = optarg;
			break;
		case 'C':
			confpath = optarg;
			break;
/*		case 'L':
			i = open("/dev/tty", O_RDWR);
			if (i != -1)
				ioctl(i, TIOCCLRVERAUTH);
			exit(i != -1);
*/
		case 'u':
			if (parseuid(optarg, &target) != 0)
				errx(1, "unknown user");
			break;
		case 'n':
			nflag = 1;
			break;
		case 's':
			sflag = 1;
			break;
		default:
			usage();
			break;
		}
	}
	argv += optind;
	argc -= optind;

	if (confpath) {
		if (sflag)
			usage();
	} else if ((!sflag && !argc) || (sflag && argc))
		usage();

	original_pw = getpwuid(uid);
	if (! original_pw)
		err(1, "getpwuid failed");
	if (strlcpy(myname, original_pw->pw_name, sizeof(myname)) >= sizeof(myname))
		errx(1, "pw_name too long");

	ngroups = getgroups(NGROUPS_MAX, groups);
	if (ngroups == -1)
		err(1, "can't get groups");
	groups[ngroups++] = getgid();

	if (sflag) {
		sh = getenv("SHELL");
		if (sh == NULL || *sh == '\0') {
			shargv[0] = strdup(original_pw->pw_shell);
			if (shargv[0] == NULL)
				err(1, NULL);
		} else
			shargv[0] = sh;
		argv = shargv;
		argc = 1;
	}

	if (confpath) {
		checkconfig(confpath, argc, argv, uid, groups, ngroups,
		    target);
		exit(1);	/* fail safe */
	}

	if (geteuid())
		errx(1, "not installed setuid");

	parseconfig(DOAS_CONF, 1);

	/* cmdline is used only for logging, no need to abort on truncate */
	(void)strlcpy(cmdline, argv[0], sizeof(cmdline));
	for (i = 1; i < argc; i++) {
		if (strlcat(cmdline, " ", sizeof(cmdline)) >= sizeof(cmdline))
			break;
		if (strlcat(cmdline, argv[i], sizeof(cmdline)) >= sizeof(cmdline))
			break;
	}

	cmd = argv[0];
	if (!permit(uid, groups, ngroups, &rule, target, cmd,
	    (const char **)argv + 1)) {
		syslog(LOG_AUTHPRIV | LOG_NOTICE,
		    "failed command for %s: %s", myname, cmdline);
		errc(1, EPERM, NULL);
	}

	if (!(rule->options & NOPASS)) {
		if (nflag)
			errx(1, "Authorization required");

#if defined(USE_BSD_AUTH) 
		authuser(myname, login_style, rule->options & PERSIST);
#elif defined(USE_PAM)
#define PAM_END(msg) do { 						\
	syslog(LOG_ERR, "%s: %s", msg, pam_strerror(pamh, pam_err)); 	\
	warnx("%s: %s", msg, pam_strerror(pamh, pam_err));		\
	pam_end(pamh, pam_err);						\
	exit(EXIT_FAILURE);						\
} while (/*CONSTCOND*/0)
		pam_handle_t *pamh = NULL;
		int pam_err;

/* #ifndef linux */
		int temp_stdin;

		/* openpam_ttyconv checks if stdin is a terminal and
		 * if it is then does not bother to open /dev/tty.
		 * The result is that PAM writes the password prompt
		 * directly to stdout.  In scenarios where stdin is a
		 * terminal, but stdout is redirected to a file
		 * e.g. by running doas ls &> ls.out interactively,
		 * the password prompt gets written to ls.out as well.
		 * By closing stdin first we forces PAM to read/write
		 * to/from the terminal directly.  We restore stdin
		 * after authenticating. */
		temp_stdin = dup(STDIN_FILENO);
		if (temp_stdin == -1)
			err(1, "dup");
		close(STDIN_FILENO);
/* #else */
		/* force password prompt to display on stderr, not stdout */
		int temp_stdout = dup(1);
		if (temp_stdout == -1)
			err(1, "dup");
		close(1);
		if (dup2(2, 1) == -1)
			err(1, "dup2");
/* #endif */

		pam_err = pam_start("doas", myname, &pamc, &pamh);
		if (pam_err != PAM_SUCCESS) {
			if (pamh != NULL)
				PAM_END("pam_start");
			syslog(LOG_ERR, "pam_start failed: %s",
			    pam_strerror(pamh, pam_err));
			errx(EXIT_FAILURE, "pam_start failed");
		}

		switch (pam_err = pam_authenticate(pamh, PAM_SILENT)) {
		case PAM_SUCCESS:
			switch (pam_err = pam_acct_mgmt(pamh, PAM_SILENT)) {
			case PAM_SUCCESS:
				break;

			case PAM_NEW_AUTHTOK_REQD:
				pam_err = pam_chauthtok(pamh,
				    PAM_SILENT|PAM_CHANGE_EXPIRED_AUTHTOK);
				if (pam_err != PAM_SUCCESS)
					PAM_END("pam_chauthtok");
				break;

			case PAM_AUTH_ERR:
			case PAM_USER_UNKNOWN:
			case PAM_MAXTRIES:
				syslog(LOG_AUTHPRIV | LOG_NOTICE,
				    "failed auth for %s", myname);
                                errx(EXIT_FAILURE, "second authentication failed");
				break;

			default:
				PAM_END("pam_acct_mgmt");
				break;
			}
			break;

		case PAM_AUTH_ERR:
		case PAM_USER_UNKNOWN:
		case PAM_MAXTRIES:
			syslog(LOG_AUTHPRIV | LOG_NOTICE,
			    "failed auth for %s", myname);
                        errx(EXIT_FAILURE, "authentication failed");
			break;

		default:
			PAM_END("pam_authenticate");
			break;
		}
		pam_end(pamh, pam_err);

#ifndef linux
		/* Re-establish stdin */
		if (dup2(temp_stdin, STDIN_FILENO) == -1)
			err(1, "dup2");
		close(temp_stdin);
#else 
		/* Re-establish stdout */
		close(1);
		if (dup2(temp_stdout, 1) == -1)
			err(1, "dup2");
#endif
#else
#error	No auth module!
#endif
	}

        /*
	if (pledge("stdio rpath getpw exec id", NULL) == -1)
		err(1, "pledge");
        */
	target_pw = getpwuid(target);
	if (! target_pw)
		errx(1, "no passwd entry for target");

#if defined(HAVE_LOGIN_CAP_H)
	if (setusercontext(NULL, target_pw, target, LOGIN_SETGROUP |
	    LOGIN_SETPRIORITY | LOGIN_SETRESOURCES | LOGIN_SETUMASK |
	    LOGIN_SETUSER) != 0)
		errx(1, "failed to set user context for target");
#endif
        /*
	if (pledge("stdio rpath exec", NULL) == -1)
		err(1, "pledge");
        */

	if (getcwd(cwdpath, sizeof(cwdpath)) == NULL)
		cwd = "(failed)";
	else
		cwd = cwdpath;

	/*
        if (pledge("stdio exec", NULL) == -1)
		err(1, "pledge");
        */
#ifndef HAVE_LOGIN_CAP_H
        /* If we effectively are root, set the UID to actually be root to avoid
           permission errors. */
        if (target != 0)
           setuid(target);
        if ( geteuid() == ROOT_UID )
           setuid(ROOT_UID);
#endif

	syslog(LOG_AUTHPRIV | LOG_INFO, "%s ran command %s as %s from %s",
	    myname, cmdline, target_pw->pw_name, cwd);

	envp = prepenv(rule, original_pw, target_pw);

	if (rule->cmd) {
		if (setenv("PATH", safepath, 1) == -1)
			err(1, "failed to set PATH '%s'", safepath);
	}
	execvpe(cmd, argv, envp);
	if (errno == ENOENT)
		errx(1, "%s: command not found", cmd);
	err(1, "%s", cmd);
}