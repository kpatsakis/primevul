static int test_path_access(const char *program, int mode)
{
	char *envpath, *p, *colon;
	int ret, our_errno = 1500; /* outside errno range */

	if (program[0] == '/' || !(envpath = getenv("PATH")))
		return access(program, mode);

	if (!(envpath = strdup(envpath))) {
		errno = ENOMEM;
		return -1;
	}

	for (p = envpath; p; p = colon + 1) {
		char *path;

		colon = strchr(p, ':');
		if (colon)
			*colon = 0;
		asprintf(&path, "%s/%s", p, program);
		ret = access(path, mode);
		free(path);
		if (!ret)
			break;

		if (ret < 0) {
			if (errno == ENOENT)
				continue;
			if (our_errno > errno)
				our_errno = errno;
		}
		if (!colon)
			break;
	}

	free(envpath);

	if (!ret)
		errno = 0;
	else
		errno = our_errno;

	return ret;
}
