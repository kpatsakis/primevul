XcursorFileLoadAllImages (FILE *file)
{
    XcursorFile	f;

    if (!file)
        return NULL;

    _XcursorStdioFileInitialize (file, &f);
    return XcursorXcFileLoadAllImages (&f);
}
