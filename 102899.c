XcursorFilenameLoadAllImages (const char *file)
{
    FILE	    *f;
    XcursorImages   *images;

    if (!file)
        return NULL;

    f = fopen (file, "r");
    if (!f)
	return NULL;
    images = XcursorFileLoadAllImages (f);
    fclose (f);
    return images;
}
