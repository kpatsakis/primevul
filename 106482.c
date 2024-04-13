XcursorLibraryLoadCursor (Display *dpy, const char *file)
{
    int		    size = XcursorGetDefaultSize (dpy);
    char	    *theme = XcursorGetTheme (dpy);
    XcursorImages   *images = XcursorLibraryLoadImages (file, theme, size);
    Cursor	    cursor;

    if (!file)
        return 0;

    if (!images)
    {
	int id = XcursorLibraryShape (file);

	if (id >= 0)
	    return _XcursorCreateFontCursor (dpy, id);
	else
	    return 0;
    }
    cursor = XcursorImagesLoadCursor (dpy, images);
    XcursorImagesDestroy (images);
#if defined HAVE_XFIXES && XFIXES_MAJOR >= 2
    XFixesSetCursorName (dpy, cursor, file);
#endif
    return cursor;
}
