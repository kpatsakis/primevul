XcursorXcFileLoadImage (XcursorFile *file, int size)
{
    XcursorFileHeader	*fileHeader;
    XcursorDim		bestSize;
    int			nsize;
    int			toc;
    XcursorImage	*image;

    if (size < 0)
	return NULL;
    fileHeader = _XcursorReadFileHeader (file);
    if (!fileHeader)
	return NULL;
    bestSize = _XcursorFindBestSize (fileHeader, (XcursorDim) size, &nsize);
    if (!bestSize)
	return NULL;
    toc = _XcursorFindImageToc (fileHeader, bestSize, 0);
    if (toc < 0)
	return NULL;
    image = _XcursorReadImage (file, fileHeader, toc);
    _XcursorFileHeaderDestroy (fileHeader);
    return image;
}
