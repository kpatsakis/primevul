int rpmVerifyFile(const rpmts ts, const rpmfi fi,
		rpmVerifyAttrs * res, rpmVerifyAttrs omitMask)
{
    rpmVerifyAttrs vfy = rpmfiVerify(fi, omitMask);
    if (res)
	*res = vfy;

    return (vfy & RPMVERIFY_LSTATFAIL) ? 1 : 0;
}
