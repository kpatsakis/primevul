rasterize(int interleaved, char* mode)
{
    register unsigned long row;
    unsigned char *newras;
    unsigned char *ras;
    TIFF *tif;
    tstrip_t strip;
    tsize_t stripsize;

    if ((newras = (unsigned char*) _TIFFmalloc(width*height+EXTRAFUDGE)) == NULL) {
        fprintf(stderr, "not enough memory for image\n");
        return;
    }
#define DRAWSEGMENT(offset, step) {			\
        for (row = offset; row < height; row += step) {	\
            _TIFFmemcpy(newras + row*width, ras, width);\
            ras += width;                            	\
        }						\
    }
    ras = raster;
    if (interleaved) {
        DRAWSEGMENT(0, 8);
        DRAWSEGMENT(4, 8);
        DRAWSEGMENT(2, 4);
        DRAWSEGMENT(1, 2);
    } else 
        DRAWSEGMENT(0, 1);
#undef DRAWSEGMENT

    tif = TIFFOpen(imagename, mode);
    if (!tif) {
	TIFFError(imagename,"Can not open output image");
	exit(-1);
    }
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32) width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32) height);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_PALETTE);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 
	rowsperstrip = TIFFDefaultStripSize(tif, rowsperstrip));
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    switch (compression) {
    case COMPRESSION_LZW:
    case COMPRESSION_DEFLATE:
	    if (predictor != 0)
		    TIFFSetField(tif, TIFFTAG_PREDICTOR, predictor);
	    break;
    }
    TIFFSetField(tif, TIFFTAG_COLORMAP, red, green, blue);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    strip = 0;
    stripsize = TIFFStripSize(tif);
    for (row=0; row<height; row += rowsperstrip) {
	if (rowsperstrip > height-row) {
	    rowsperstrip = height-row;
	    stripsize = TIFFVStripSize(tif, rowsperstrip);
	}
	if (TIFFWriteEncodedStrip(tif, strip, newras+row*width, stripsize) < 0)
	    break;
	strip++;
    }
    TIFFClose(tif);

    _TIFFfree(newras);
}
