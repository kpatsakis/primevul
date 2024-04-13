static void fbFetchExternalAlpha(PicturePtr pict, int x, int y, int width, CARD32 *buffer, CARD32 *mask, CARD32 maskBits)
{
    int i;
    CARD32 _alpha_buffer[SCANLINE_BUFFER_LENGTH];
    CARD32 *alpha_buffer = _alpha_buffer;

    if (!pict->alphaMap) {
        fbFetchTransformed(pict, x, y, width, buffer, mask, maskBits);
	return;
    }
    if (width > SCANLINE_BUFFER_LENGTH)
        alpha_buffer = (CARD32 *) malloc(width*sizeof(CARD32));

    fbFetchTransformed(pict, x, y, width, buffer, mask, maskBits);
    fbFetchTransformed(pict->alphaMap, x - pict->alphaOrigin.x,
		       y - pict->alphaOrigin.y, width, alpha_buffer,
		       mask, maskBits);
    for (i = 0; i < width; ++i) {
        if (!mask || mask[i] & maskBits)
	{
	    int a = alpha_buffer[i]>>24;
	    WRITE(buffer + i, (a << 24)
		  | (div_255(Red(READ(buffer + i)) * a) << 16)
		  | (div_255(Green(READ(buffer + i)) * a) << 8)
		  | (div_255(Blue(READ(buffer + i)) * a)));
	}
    }

    if (alpha_buffer != _alpha_buffer)
        free(alpha_buffer);
}
