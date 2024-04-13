static int __translate_perms(int flags) {
	int perms = 0;
	if (flags & IS_RX) {
		if (flags & IS_DATA) {
			perms = R_PERM_R;
		} else {
			perms = R_PERM_X;
		}
	}
	if (!perms) {
		perms = R_PERM_RWX;
	}
	return perms;
}