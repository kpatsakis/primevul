XcursorXcFileSave (XcursorFile		    *file,
		   const XcursorComments    *comments,
		   const XcursorImages	    *images)
{
    XcursorFileHeader	*fileHeader;
    XcursorUInt		position;
    int			n;
    int			toc;

    if (!file || !comments || !images)
        return XcursorFalse;

    fileHeader = _XcursorFileHeaderCreate (comments->ncomment + images->nimage);
    if (!fileHeader)
	return XcursorFalse;

    position = _XcursorFileHeaderLength (fileHeader);

    /*
     * Compute the toc.  Place the images before the comments
     * as they're more often read
     */

    toc = 0;
    for (n = 0; n < images->nimage; n++)
    {
	fileHeader->tocs[toc].type = XCURSOR_IMAGE_TYPE;
	fileHeader->tocs[toc].subtype = images->images[n]->size;
	fileHeader->tocs[toc].position = position;
	position += _XcursorImageLength (images->images[n]);
	toc++;
    }

    for (n = 0; n < comments->ncomment; n++)
    {
	fileHeader->tocs[toc].type = XCURSOR_COMMENT_TYPE;
	fileHeader->tocs[toc].subtype = comments->comments[n]->comment_type;
	fileHeader->tocs[toc].position = position;
	position += _XcursorCommentLength (comments->comments[n]);
	toc++;
    }

    /*
     * Write the header and the toc
     */
    if (!_XcursorWriteFileHeader (file, fileHeader))
	goto bail;

    /*
     * Write the images
     */
    toc = 0;
    for (n = 0; n < images->nimage; n++)
    {
	if (!_XcursorWriteImage (file, fileHeader, toc, images->images[n]))
	    goto bail;
	toc++;
    }

    /*
     * Write the comments
     */
    for (n = 0; n < comments->ncomment; n++)
    {
	if (!_XcursorWriteComment (file, fileHeader, toc, comments->comments[n]))
	    goto bail;
	toc++;
    }

    _XcursorFileHeaderDestroy (fileHeader);
    return XcursorTrue;
bail:
    _XcursorFileHeaderDestroy (fileHeader);
    return XcursorFalse;
}
