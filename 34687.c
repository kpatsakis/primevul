bdfReadFont(FontPtr pFont, FontFilePtr file,
	    int bit, int byte, int glyph, int scan)
{
    bdfFileState state;
    xCharInfo  *min,
               *max;
    BitmapFontPtr  bitmapFont;

    pFont->fontPrivate = 0;

    bzero(&state, sizeof(bdfFileState));
    bdfFileLineNum = 0;

    if (!bdfReadHeader(file, &state))
	goto BAILOUT;

    bitmapFont = calloc(1, sizeof(BitmapFontRec));
    if (!bitmapFont) {
	bdfError("Couldn't allocate bitmapFontRec (%d)\n",
		 (int) sizeof(BitmapFontRec));
	goto BAILOUT;
    }

    pFont->fontPrivate = (pointer) bitmapFont;
    bitmapFont->metrics = 0;
    bitmapFont->ink_metrics = 0;
    bitmapFont->bitmaps = 0;
    bitmapFont->encoding = 0;
    bitmapFont->pDefault = NULL;

    bitmapFont->bitmapExtra = calloc(1, sizeof(BitmapExtraRec));
    if (!bitmapFont->bitmapExtra) {
	bdfError("Couldn't allocate bitmapExtra (%d)\n",
		 (int) sizeof(BitmapExtraRec));
        goto BAILOUT;
    }

    bitmapFont->bitmapExtra->glyphNames = 0;
    bitmapFont->bitmapExtra->sWidths = 0;

    if (!bdfReadProperties(file, pFont, &state))
	goto BAILOUT;

    if (!bdfReadCharacters(file, pFont, &state, bit, byte, glyph, scan))
	goto BAILOUT;

    if (state.haveDefaultCh) {
	unsigned int r, c, cols;

	r = pFont->info.defaultCh >> 8;
	c = pFont->info.defaultCh & 0xFF;
	if (pFont->info.firstRow <= r && r <= pFont->info.lastRow &&
		pFont->info.firstCol <= c && c <= pFont->info.lastCol) {
	    cols = pFont->info.lastCol - pFont->info.firstCol + 1;
	    r = r - pFont->info.firstRow;
	    c = c - pFont->info.firstCol;
	    bitmapFont->pDefault = ACCESSENCODING(bitmapFont->encoding,
                                                 r * cols + c);
	}
    }
    pFont->bit = bit;
    pFont->byte = byte;
    pFont->glyph = glyph;
    pFont->scan = scan;
    pFont->info.anamorphic = FALSE;
    pFont->info.cachable = TRUE;
    bitmapComputeFontBounds(pFont);
    if (FontCouldBeTerminal(&pFont->info)) {
	bdfPadToTerminal(pFont);
	bitmapComputeFontBounds(pFont);
    }
    FontComputeInfoAccelerators(&pFont->info);
    if (bitmapFont->bitmapExtra)
	FontComputeInfoAccelerators(&bitmapFont->bitmapExtra->info);
    if (pFont->info.constantMetrics) {
      if (!bitmapAddInkMetrics(pFont)) {
        bdfError("Failed to add bitmap ink metrics\n");
        goto BAILOUT;
      }
    }
    if (bitmapFont->bitmapExtra)
	bitmapFont->bitmapExtra->info.inkMetrics = pFont->info.inkMetrics;

    bitmapComputeFontInkBounds(pFont);
/*    ComputeFontAccelerators (pFont); */

    /* generate properties */
    min = &pFont->info.ink_minbounds;
    max = &pFont->info.ink_maxbounds;
    if (state.xHeightProp && (state.xHeightProp->value == -1))
	state.xHeightProp->value = state.exHeight ?
	    state.exHeight : min->ascent;

    if (state.quadWidthProp && (state.quadWidthProp->value == -1))
	state.quadWidthProp->value = state.digitCount ?
	    (INT32) (state.digitWidths / state.digitCount) :
	    (min->characterWidth + max->characterWidth) / 2;

    if (state.weightProp && (state.weightProp->value == -1))
	state.weightProp->value = bitmapComputeWeight(pFont);

    pFont->get_glyphs = bitmapGetGlyphs;
    pFont->get_metrics = bitmapGetMetrics;
    pFont->unload_font = bdfUnloadFont;
    pFont->unload_glyphs = NULL;
    return Successful;
BAILOUT:
    if (pFont->fontPrivate)
	bdfFreeFontBits (pFont);
    return AllocError;
}
