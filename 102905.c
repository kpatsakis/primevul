XcursorXcFileLoad (XcursorFile	    *file,
		   XcursorComments  **commentsp,
		   XcursorImages    **imagesp)
{
    XcursorFileHeader	*fileHeader;
    int			nimage;
    int			ncomment;
    XcursorImages	*images;
    XcursorImage	*image;
    XcursorComment	*comment;
    XcursorComments	*comments;
    int			toc;

    if (!file)
        return 0;
    fileHeader = _XcursorReadFileHeader (file);
    if (!fileHeader)
	return 0;
    nimage = 0;
    ncomment = 0;
    for (toc = 0; toc < fileHeader->ntoc; toc++)
    {
	switch (fileHeader->tocs[toc].type) {
	case XCURSOR_COMMENT_TYPE:
	    ncomment++;
	    break;
	case XCURSOR_IMAGE_TYPE:
	    nimage++;
	    break;
	}
    }
    images = XcursorImagesCreate (nimage);
    if (!images)
	return 0;
    comments = XcursorCommentsCreate (ncomment);
    if (!comments)
    {
	XcursorImagesDestroy (images);
	return 0;
    }
    for (toc = 0; toc < fileHeader->ntoc; toc++)
    {
	switch (fileHeader->tocs[toc].type) {
	case XCURSOR_COMMENT_TYPE:
	    comment = _XcursorReadComment (file, fileHeader, toc);
	    if (comment)
	    {
		comments->comments[comments->ncomment] = comment;
		comments->ncomment++;
	    }
	    break;
	case XCURSOR_IMAGE_TYPE:
	    image = _XcursorReadImage (file, fileHeader, toc);
	    if (image)
	    {
		images->images[images->nimage] = image;
		images->nimage++;
	    }
	    break;
	}
    }
    _XcursorFileHeaderDestroy (fileHeader);
    if (images->nimage != nimage || comments->ncomment != ncomment)
    {
	XcursorImagesDestroy (images);
	XcursorCommentsDestroy (comments);
	images = NULL;
	comments = NULL;
	return XcursorFalse;
    }
    *imagesp = images;
    *commentsp = comments;
    return XcursorTrue;
}
