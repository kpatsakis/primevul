XcursorFileSave (FILE *			file,
		 const XcursorComments	*comments,
		 const XcursorImages	*images)
{
    XcursorFile	    f;

    if (!file || !comments || !images)
        return XcursorFalse;

    _XcursorStdioFileInitialize (file, &f);
    return XcursorXcFileSave (&f, comments, images) && fflush (file) != EOF;
}
