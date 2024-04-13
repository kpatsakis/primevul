static int get_option_gid(substring_t args[], kgid_t *result)
{
	unsigned long value;
	kgid_t gid;
	int rc;

	rc = get_option_ul(args, &value);
	if (rc)
		return rc;

	gid = make_kgid(current_user_ns(), value);
	if (!gid_valid(gid))
		return -EINVAL;

	*result = gid;
	return 0;
}
