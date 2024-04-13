XcursorFilenameSave (const char		    *file,
		     const XcursorComments  *comments,
		     const XcursorImages    *images)
{
    FILE	    *f;
    XcursorBool	    ret;

    if (!file || !comments || !images)
        return XcursorFalse;

    f = fopen (file, "w");
    if (!f)
	return 0;
    ret = XcursorFileSave (f, comments, images);
    return fclose (f) != EOF && ret;
}
