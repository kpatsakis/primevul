_XcursorWriteImage (XcursorFile		*file,
		    XcursorFileHeader	*fileHeader,
		    int			toc,
		    XcursorImage	*image)
{
    XcursorChunkHeader	chunkHeader;
    int			n;
    XcursorPixel	*p;

    if (!file || !fileHeader || !image)
        return XcursorFalse;

    /* sanity check data */
    if (image->width > XCURSOR_IMAGE_MAX_SIZE  ||
	image->height > XCURSOR_IMAGE_MAX_SIZE)
	return XcursorFalse;
    if (image->width == 0 || image->height == 0)
	return XcursorFalse;
    if (image->xhot > image->width || image->yhot > image->height)
	return XcursorFalse;

    /* write chunk header */
    chunkHeader.header = XCURSOR_IMAGE_HEADER_LEN;
    chunkHeader.type = XCURSOR_IMAGE_TYPE;
    chunkHeader.subtype = image->size;
    chunkHeader.version = XCURSOR_IMAGE_VERSION;

    if (!_XcursorFileWriteChunkHeader (file, fileHeader, toc, &chunkHeader))
	return XcursorFalse;

    /* write extra image header fields */
    if (!_XcursorWriteUInt (file, image->width))
	return XcursorFalse;
    if (!_XcursorWriteUInt (file, image->height))
	return XcursorFalse;
    if (!_XcursorWriteUInt (file, image->xhot))
	return XcursorFalse;
    if (!_XcursorWriteUInt (file, image->yhot))
	return XcursorFalse;
    if (!_XcursorWriteUInt (file, image->delay))
	return XcursorFalse;

    /* write the image */
    n = image->width * image->height;
    p = image->pixels;
    while (n--)
    {
	if (!_XcursorWriteUInt (file, *p))
	    return XcursorFalse;
	p++;
    }
    return XcursorTrue;
}
