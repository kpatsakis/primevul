char * rpmVerifyString(uint32_t verifyResult, const char *pad)
{
    char *fmt = NULL;
    rasprintf(&fmt, "%s%s%s%s%s%s%s%s%s",
		_verify(RPMVERIFY_FILESIZE, "S", pad),
		_verify(RPMVERIFY_MODE, "M", pad),
		_verifyfile(RPMVERIFY_FILEDIGEST, "5", pad),
		_verify(RPMVERIFY_RDEV, "D", pad),
		_verifylink(RPMVERIFY_LINKTO, "L", pad),
		_verify(RPMVERIFY_USER, "U", pad),
		_verify(RPMVERIFY_GROUP, "G", pad),
		_verify(RPMVERIFY_MTIME, "T", pad),
		_verify(RPMVERIFY_CAPS, "P", pad));
		
    return fmt;
}
