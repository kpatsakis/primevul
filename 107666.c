static int verifyDependencies(rpmts ts, Header h)
{
    rpmps ps;
    rpmte te;
    int rc;

    rpmtsEmpty(ts);
    (void) rpmtsAddInstallElement(ts, h, NULL, 0, NULL);

    (void) rpmtsCheck(ts);
    te = rpmtsElement(ts, 0);
    ps = rpmteProblems(te);
    rc = rpmpsNumProblems(ps);

    if (rc > 0) {
	rpmlog(RPMLOG_NOTICE, _("Unsatisfied dependencies for %s:\n"),
	       rpmteNEVRA(te));
	rpmpsi psi = rpmpsInitIterator(ps);
	rpmProblem p;

	while ((p = rpmpsiNext(psi)) != NULL) {
	    char * ps = rpmProblemString(p);
	    rpmlog(RPMLOG_NOTICE, "\t%s\n", ps);
	    free(ps);
	}
	rpmpsFreeIterator(psi);
    }
    rpmpsFree(ps);
    rpmtsEmpty(ts);

    return rc;
}
