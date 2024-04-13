bdfReadHeader(FontFilePtr file, bdfFileState *pState)
{
    unsigned char *line;
    char        namebuf[BDFLINELEN];
    unsigned char        lineBuf[BDFLINELEN];

    line = bdfGetLine(file, lineBuf, BDFLINELEN);
    if (!line || sscanf((char *) line, "STARTFONT %s", namebuf) != 1 ||
	    !bdfStrEqual(namebuf, "2.1")) {
	bdfError("bad 'STARTFONT'\n");
	return (FALSE);
    }
    line = bdfGetLine(file, lineBuf, BDFLINELEN);
    if (!line || sscanf((char *) line, "FONT %[^\n]", pState->fontName) != 1) {
	bdfError("bad 'FONT'\n");
	return (FALSE);
    }
    line = bdfGetLine(file, lineBuf, BDFLINELEN);
    if (!line || !bdfIsPrefix(line, "SIZE")) {
	bdfError("missing 'SIZE'\n");
	return (FALSE);
    }
    if (sscanf((char *) line, "SIZE %f%d%d", &pState->pointSize,
	       &pState->resolution_x, &pState->resolution_y) != 3) {
	bdfError("bad 'SIZE'\n");
	return (FALSE);
    }
    if (pState->pointSize < 1 ||
	pState->resolution_x < 1 || pState->resolution_y < 1) {
	bdfError("SIZE values must be > 0\n");
	return (FALSE);
    }
    line = bdfGetLine(file, lineBuf, BDFLINELEN);
    if (!line || !bdfIsPrefix(line, "FONTBOUNDINGBOX")) {
	bdfError("missing 'FONTBOUNDINGBOX'\n");
	return (FALSE);
    }
    return (TRUE);
}
