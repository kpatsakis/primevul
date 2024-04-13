XcursorShapeLoadCursors (Display *dpy, unsigned int shape)
{
    unsigned int    id = shape >> 1;

    if (id < NUM_STANDARD_NAMES)
	return XcursorLibraryLoadCursors (dpy, STANDARD_NAME (id));
    else
	return NULL;
}
