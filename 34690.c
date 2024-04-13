bdfSkipBitmap(FontFilePtr file, int height)
{
    unsigned char *line;
    int         i = 0;
    unsigned char        lineBuf[BDFLINELEN];

    do {
	line = bdfGetLine(file, lineBuf, BDFLINELEN);
	i++;
    } while (line && !bdfIsPrefix(line, "ENDCHAR") && i <= height);

    if (i > 1 && line && !bdfIsPrefix(line, "ENDCHAR")) {
	bdfError("Error in bitmap, missing 'ENDCHAR'\n");
	return (FALSE);
    }
    return (TRUE);
}
