XcursorXcFileLoadAllImages (XcursorFile *file)
{
    XcursorFileHeader	*fileHeader;
    XcursorImage	*image;
    XcursorImages	*images;
    int			nimage;
    int			n;
    int			toc;

    if (!file)
        return NULL;

    fileHeader = _XcursorReadFileHeader (file);
    if (!fileHeader)
	return NULL;
    nimage = 0;
    for (n = 0; n < fileHeader->ntoc; n++)
    {
	switch (fileHeader->tocs[n].type) {
	case XCURSOR_IMAGE_TYPE:
	    nimage++;
	    break;
	}
    }
    images = XcursorImagesCreate (nimage);
    if (!images)
    {
	_XcursorFileHeaderDestroy (fileHeader);
	return NULL;
    }
    for (toc = 0; toc < fileHeader->ntoc; toc++)
    {
	switch (fileHeader->tocs[toc].type) {
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
    if (images->nimage != nimage)
    {
	XcursorImagesDestroy (images);
	images = NULL;
    }
    return images;
}
