static int verifyHeader(rpmts ts, Header h, rpmVerifyAttrs omitMask,
			rpmfileAttrs skipAttrs)
{
    rpmVerifyAttrs verifyResult = 0;
    rpmVerifyAttrs verifyAll = 0; /* assume no problems */
    rpmfi fi = rpmfiNew(ts, h, RPMTAG_BASENAMES, RPMFI_FLAGS_VERIFY);

    if (fi == NULL)
	return 1;

    rpmfiInit(fi, 0);
    while (rpmfiNext(fi) >= 0) {
	rpmfileAttrs fileAttrs = rpmfiFFlags(fi);
	char *buf = NULL, *attrFormat;
	const char *fstate = NULL;
	char ac;

	/* Skip on attributes (eg from --noghost) */
	if (skipAttrs & fileAttrs)
	    continue;

	verifyResult = rpmfiVerify(fi, omitMask);

	/* Filter out timestamp differences of shared files */
	if (verifyResult & RPMVERIFY_MTIME) {
	    rpmdbMatchIterator mi;
	    mi = rpmtsInitIterator(ts, RPMDBI_BASENAMES, rpmfiFN(fi), 0);
	    if (rpmdbGetIteratorCount(mi) > 1) 
		verifyResult &= ~RPMVERIFY_MTIME;
	    rpmdbFreeIterator(mi);
	}

	/* State is only meaningful for installed packages */
	if (headerGetInstance(h))
	    fstate = stateStr(rpmfiFState(fi));

	attrFormat = rpmFFlagsString(fileAttrs, "");
	ac = rstreq(attrFormat, "") ? ' ' : attrFormat[0];
	if (verifyResult & RPMVERIFY_LSTATFAIL) {
	    if (!(fileAttrs & (RPMFILE_MISSINGOK|RPMFILE_GHOST)) || rpmIsVerbose()) {
		rasprintf(&buf, _("missing   %c %s"), ac, rpmfiFN(fi));
		if ((verifyResult & RPMVERIFY_LSTATFAIL) != 0 &&
		    errno != ENOENT) {
		    char *app;
		    rasprintf(&app, " (%s)", strerror(errno));
		    rstrcat(&buf, app);
		    free(app);
		}
	    }
	} else if (verifyResult || fstate || rpmIsVerbose()) {
	    char *verifyFormat = rpmVerifyString(verifyResult, ".");
	    rasprintf(&buf, "%s  %c %s", verifyFormat, ac, rpmfiFN(fi));
	    free(verifyFormat);
	}
	free(attrFormat);

	if (buf) {
	    if (fstate)
		buf = rstrscat(&buf, " (", fstate, ")", NULL);
	    rpmlog(RPMLOG_NOTICE, "%s\n", buf);
	    buf = _free(buf);
	}
	verifyAll |= verifyResult;
    }
    rpmfiFree(fi);
	
    return (verifyAll != 0) ? 1 : 0;
}
