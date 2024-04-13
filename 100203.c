static int line_cb(
	const git_diff_delta *delta,
	const git_diff_hunk *hunk,
	const git_diff_line *line,
	void *payload)
{
	GIT_UNUSED(delta);
	GIT_UNUSED(hunk);
	GIT_UNUSED(line);
	GIT_UNUSED(payload);

	cl_fail("did not expect line callback");
	return 0;
}
