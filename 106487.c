XcursorScanTheme (const char *theme, const char *name)
{
    FILE	*f = NULL;
    char	*full;
    char	*dir;
    const char  *path;
    char	*inherits = NULL;
    const char	*i;

    if (!theme || !name)
        return NULL;

    /*
     * XCURSOR_CORE_THEME is a magic name; cursors from the core set
     * are never found in any directory.  Instead, a magic value is
     * returned which truncates any search so that overlying functions
     * can switch to equivalent core cursors
     */
    if (!strcmp (theme, XCURSOR_CORE_THEME) && XcursorLibraryShape (name) >= 0)
	return XCURSOR_SCAN_CORE;
    /*
     * Scan this theme
     */
    for (path = XcursorLibraryPath ();
	 path && f == NULL;
	 path = _XcursorNextPath (path))
    {
	dir = _XcursorBuildThemeDir (path, theme);
	if (dir)
	{
	    full = _XcursorBuildFullname (dir, "cursors", name);
	    if (full)
	    {
		f = fopen (full, "r");
		free (full);
	    }
	    if (!f && !inherits)
	    {
		full = _XcursorBuildFullname (dir, "", "index.theme");
		if (full)
		{
		    inherits = _XcursorThemeInherits (full);
		    free (full);
		}
	    }
	    free (dir);
	}
    }
    /*
     * Recurse to scan inherited themes
     */
    for (i = inherits; i && f == NULL; i = _XcursorNextPath (i))
	f = XcursorScanTheme (i, name);
    if (inherits != NULL)
	free (inherits);
    return f;
}
