XcursorFileLoadImage (FILE *file, int size)
{
    XcursorFile	f;

    if (!file)
        return NULL;

    _XcursorStdioFileInitialize (file, &f);
    return XcursorXcFileLoadImage (&f, size);
}
