XcursorLibraryLoadImage (const char *file, const char *theme, int size)
{
    FILE	    *f = NULL;
    XcursorImage    *image = NULL;

    if (!file)
        return NULL;

    if (theme)
	f = XcursorScanTheme (theme, file);
    if (!f)
	f = XcursorScanTheme ("default", file);
    if (f == XCURSOR_SCAN_CORE)
	return NULL;
    if (f)
    {
	image = XcursorFileLoadImage (f, size);
	fclose (f);
    }
    return image;
}
