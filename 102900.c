XcursorFilenameLoadImage (const char *file, int size)
{
    FILE	    *f;
    XcursorImage    *image;

    if (!file || size < 0)
        return NULL;

    f = fopen (file, "r");
    if (!f)
	return NULL;
    image = XcursorFileLoadImage (f, size);
    fclose (f);
    return image;
}
