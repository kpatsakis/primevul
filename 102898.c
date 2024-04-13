XcursorFilenameLoad (const char		*file,
		     XcursorComments	**commentsp,
		     XcursorImages	**imagesp)
{
    FILE	    *f;
    XcursorBool	    ret;

    if (!file)
        return XcursorFalse;

    f = fopen (file, "r");
    if (!f)
	return 0;
    ret = XcursorFileLoad (f, commentsp, imagesp);
    fclose (f);
    return ret;
}
