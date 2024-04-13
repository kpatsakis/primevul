static int get_option_uid(substring_t args[], kuid_t *result)
{
	unsigned long value;
	kuid_t uid;
	int rc;

	rc = get_option_ul(args, &value);
	if (rc)
		return rc;

	uid = make_kuid(current_user_ns(), value);
	if (!uid_valid(uid))
		return -EINVAL;

	*result = uid;
	return 0;
}
