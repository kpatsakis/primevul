XcursorFilenameSaveImages (const char *file, const XcursorImages *images)
{
    FILE	    *f;
    XcursorBool	    ret;

    if (!file || !images)
        return XcursorFalse;

    f = fopen (file, "w");
    if (!f)
	return 0;
    ret = XcursorFileSaveImages (f, images);
    return fclose (f) != EOF && ret;
}
