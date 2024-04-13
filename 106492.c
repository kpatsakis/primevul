_XcursorAddPathElt (char *path, const char *elt, int len)
{
    size_t    pathlen = strlen (path);

    /* append / if the path doesn't currently have one */
    if (path[0] == '\0' || path[pathlen - 1] != '/')
    {
	strcat (path, "/");
	pathlen++;
    }
    if (len == -1)
	len = strlen (elt);
    /* strip leading slashes */
    while (len && elt[0] == '/')
    {
	elt++;
	len--;
    }
    strncpy (path + pathlen, elt, len);
    path[pathlen + len] = '\0';
}
