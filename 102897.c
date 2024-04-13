XcursorFileSaveImages (FILE *file, const XcursorImages *images)
{
    XcursorComments *comments;
    XcursorFile	    f;
    XcursorBool	    ret;

    if (!file || !images)
	return 0;
    if ((comments = XcursorCommentsCreate (0)) == NULL)
	return 0;
    _XcursorStdioFileInitialize (file, &f);
    ret = XcursorXcFileSave (&f, comments, images) && fflush (file) != EOF;
    XcursorCommentsDestroy (comments);
    return ret;
}
