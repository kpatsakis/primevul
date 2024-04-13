XcursorShapeLoadCursor (Display *dpy, unsigned int shape)
{
    unsigned int    id = shape >> 1;

    if (id < NUM_STANDARD_NAMES)
	return XcursorLibraryLoadCursor (dpy, STANDARD_NAME (id));
    else
	return 0;
}
