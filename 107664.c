int showVerifyPackage(QVA_t qva, rpmts ts, Header h)
{
    rpmVerifyAttrs omitMask = ((qva->qva_flags & VERIFY_ATTRS) ^ VERIFY_ATTRS);
    int ec = 0;
    int rc;

    if (qva->qva_flags & VERIFY_DEPS) {
	if ((rc = verifyDependencies(ts, h)) != 0)
	    ec = rc;
    }
    if (qva->qva_flags & VERIFY_FILES) {
	if ((rc = verifyHeader(ts, h, omitMask, qva->qva_fflags)) != 0)
	    ec = rc;
    }
    if (qva->qva_flags & VERIFY_SCRIPT) {
	if ((rc = rpmVerifyScript(ts, h)) != 0)
	    ec = rc;
    }

    return ec;
}
