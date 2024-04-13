MakeBitmapsFromFont(FontPtr pFont, int first, int count, int list_base)
{
    unsigned long   i, nglyphs;
    CARD8	    chs[2];		/* the font index we are going after */
    CharInfoPtr	    pci;
    int rv;				/* return value */
    int encoding = (FONTLASTROW(pFont) == 0) ? Linear16Bit : TwoD16Bit;
    
    CALL_PixelStorei( GET_DISPATCH(), (GL_UNPACK_SWAP_BYTES, FALSE) );
    CALL_PixelStorei( GET_DISPATCH(), (GL_UNPACK_LSB_FIRST, BITMAP_BIT_ORDER == LSBFirst) );
    CALL_PixelStorei( GET_DISPATCH(), (GL_UNPACK_ROW_LENGTH, 0) );
    CALL_PixelStorei( GET_DISPATCH(), (GL_UNPACK_SKIP_ROWS, 0) );
    CALL_PixelStorei( GET_DISPATCH(), (GL_UNPACK_SKIP_PIXELS, 0) );
    CALL_PixelStorei( GET_DISPATCH(), (GL_UNPACK_ALIGNMENT, GLYPHPADBYTES) );
    for (i=0; i < count; i++) {
	chs[0] = (first + i) >> 8;	/* high byte is first byte */
	chs[1] = first + i;

	(*pFont->get_glyphs)(pFont, 1, chs, (FontEncoding)encoding, 
		&nglyphs, &pci);

	/*
	** Define a display list containing just a glBitmap() call.
	*/
	CALL_NewList( GET_DISPATCH(), (list_base + i, GL_COMPILE) );
	if (nglyphs ) {
	    rv = __glXMakeBitmapFromGlyph(pFont, pci);
	    if (rv) {
		return rv;
	    }
	}
	CALL_EndList( GET_DISPATCH(), () );
    }
    return Success;
}
