XcursorLibraryLoadCursors (Display *dpy, const char *file)
{
    int		    size = XcursorGetDefaultSize (dpy);
    char	    *theme = XcursorGetTheme (dpy);
    XcursorImages   *images = XcursorLibraryLoadImages (file, theme, size);
    XcursorCursors  *cursors;

    if (!file)
        return NULL;

    if (!images)
    {
	int id = XcursorLibraryShape (file);

	if (id >= 0)
	{
	    cursors = XcursorCursorsCreate (dpy, 1);
	    if (cursors)
	    {
		cursors->cursors[0] = _XcursorCreateFontCursor (dpy, id);
		if (cursors->cursors[0] == None)
		{
		    XcursorCursorsDestroy (cursors);
		    cursors = NULL;
		}
		else
		    cursors->ncursor = 1;
	    }
	}
	else
	    cursors = NULL;
    }
    else
    {
	cursors = XcursorImagesLoadCursors (dpy, images);
	XcursorImagesDestroy (images);
    }
    return cursors;
}
