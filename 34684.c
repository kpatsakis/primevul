bdfFreeFontBits(FontPtr pFont)
{
    BitmapFontPtr  bitmapFont;
    BitmapExtraPtr bitmapExtra;
    int         i, nencoding;

    bitmapFont = (BitmapFontPtr) pFont->fontPrivate;
    bitmapExtra = (BitmapExtraPtr) bitmapFont->bitmapExtra;
    free(bitmapFont->ink_metrics);
    if(bitmapFont->encoding) {
        nencoding = (pFont->info.lastCol - pFont->info.firstCol + 1) *
	    (pFont->info.lastRow - pFont->info.firstRow + 1);
        for(i=0; i<NUM_SEGMENTS(nencoding); i++)
            free(bitmapFont->encoding[i]);
    }
    free(bitmapFont->encoding);
    for (i = 0; i < bitmapFont->num_chars; i++)
	free(bitmapFont->metrics[i].bits);
    free(bitmapFont->metrics);
    if (bitmapExtra)
    {
	free (bitmapExtra->glyphNames);
	free (bitmapExtra->sWidths);
	free (bitmapExtra);
    }
    free(pFont->info.props);
    free(bitmapFont);
}
