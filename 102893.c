XcursorFileLoad (FILE		    *file,
		 XcursorComments    **commentsp,
		 XcursorImages	    **imagesp)
{
    XcursorFile	f;

    if (!file || !commentsp || !imagesp)
        return XcursorFalse;

    _XcursorStdioFileInitialize (file, &f);
    return XcursorXcFileLoad (&f, commentsp, imagesp);
}
