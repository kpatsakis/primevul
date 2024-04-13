ProcRenderFreeGlyphs (ClientPtr client)
{
    REQUEST(xRenderFreeGlyphsReq);
    GlyphSetPtr     glyphSet;
    int		    rc, nglyph;
    CARD32	    *gids;
    CARD32	    glyph;

    REQUEST_AT_LEAST_SIZE(xRenderFreeGlyphsReq);
    rc = dixLookupResourceByType((pointer *)&glyphSet, stuff->glyphset, GlyphSetType,
			   client, DixRemoveAccess);
    if (rc != Success)
    {
	client->errorValue = stuff->glyphset;
	return rc;
    }
    nglyph = bytes_to_int32((client->req_len << 2) - sizeof (xRenderFreeGlyphsReq));
    gids = (CARD32 *) (stuff + 1);
    while (nglyph-- > 0)
    {
	glyph = *gids++;
	if (!DeleteGlyph (glyphSet, glyph))
	{
	    client->errorValue = glyph;
	    return RenderErrBase + BadGlyph;
	}
    }
    return Success;
}
