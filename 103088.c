XFreeFontPath (char **list)
{
	if (list != NULL) {
		Xfree (list[0]-1);
		Xfree (list);
	}
	return 1;
}
