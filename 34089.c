CheckUserAuthorization(void)
{
#ifdef USE_PAM
    static struct pam_conv conv = {
	misc_conv,
	NULL
    };

    pam_handle_t *pamh = NULL;
    struct passwd *pw;
    int retval;

    if (getuid() != geteuid()) {
	pw = getpwuid(getuid());
	if (pw == NULL)
	    FatalError("getpwuid() failed for uid %d\n", getuid());

	retval = pam_start("xserver", pw->pw_name, &conv, &pamh);
	if (retval != PAM_SUCCESS)
	    FatalError("pam_start() failed.\n"
			"\tMissing or mangled PAM config file or module?\n");

	retval = pam_authenticate(pamh, 0);
	if (retval != PAM_SUCCESS) {
	    pam_end(pamh, retval);
	    FatalError("PAM authentication failed, cannot start X server.\n"
			"\tPerhaps you do not have console ownership?\n");
	}

	retval = pam_acct_mgmt(pamh, 0);
	if (retval != PAM_SUCCESS) {
	    pam_end(pamh, retval);
	    FatalError("PAM authentication failed, cannot start X server.\n"
			"\tPerhaps you do not have console ownership?\n");
	}

	/* this is not a session, so do not do session management */
	pam_end(pamh, PAM_SUCCESS);
    }
#endif
}
