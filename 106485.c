XcursorLibraryLoadImages (const char *file, const char *theme, int size)
{
    FILE	    *f = NULL;
    XcursorImages   *images = NULL;

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
	images = XcursorFileLoadImages (f, size);
	if (images)
	    XcursorImagesSetName (images, file);
	fclose (f);
    }
    return images;
}
