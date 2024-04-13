bdfReadProperties(FontFilePtr file, FontPtr pFont, bdfFileState *pState)
{
    int         nProps, props_left,
                nextProp;
    char       *stringProps;
    FontPropPtr props;
    char        namebuf[BDFLINELEN],
                secondbuf[BDFLINELEN],
                thirdbuf[BDFLINELEN];
    unsigned char *line;
    unsigned char        lineBuf[BDFLINELEN];
    BitmapFontPtr  bitmapFont = (BitmapFontPtr) pFont->fontPrivate;

    line = bdfGetLine(file, lineBuf, BDFLINELEN);
    if (!line || !bdfIsPrefix(line, "STARTPROPERTIES")) {
	bdfError("missing 'STARTPROPERTIES'\n");
	return (FALSE);
    }
    if (sscanf((char *) line, "STARTPROPERTIES %d", &nProps) != 1) {
	bdfError("bad 'STARTPROPERTIES'\n");
	return (FALSE);
    }
    pFont->info.isStringProp = NULL;
    pFont->info.props = NULL;
    pFont->info.nprops = 0;

    stringProps = malloc((nProps + BDF_GENPROPS) * sizeof(char));
    pFont->info.isStringProp = stringProps;
    if (stringProps == NULL) {
	bdfError("Couldn't allocate stringProps (%d*%d)\n",
		 (nProps + BDF_GENPROPS), (int) sizeof(Bool));
	goto BAILOUT;
    }
    pFont->info.props = props = calloc(nProps + BDF_GENPROPS,
				       sizeof(FontPropRec));
    if (props == NULL) {
	bdfError("Couldn't allocate props (%d*%d)\n", nProps + BDF_GENPROPS,
						   (int) sizeof(FontPropRec));
	goto BAILOUT;
    }

    nextProp = 0;
    props_left = nProps;
    while (props_left-- > 0) {
	line = bdfGetLine(file, lineBuf, BDFLINELEN);
	if (line == NULL || bdfIsPrefix(line, "ENDPROPERTIES")) {
	    bdfError("\"STARTPROPERTIES %d\" followed by only %d properties\n",
		     nProps, nProps - props_left - 1);
	    goto BAILOUT;
	}
	while (*line && isspace(*line))
	    line++;

	switch (sscanf((char *) line, "%s%s%s", namebuf, secondbuf, thirdbuf)) {
	default:
	    bdfError("missing '%s' parameter value\n", namebuf);
	    goto BAILOUT;

	case 2:
	    /*
	     * Possibilites include: valid quoted string with no white space
	     * valid integer value invalid value
	     */
	    if (secondbuf[0] == '"') {
		stringProps[nextProp] = TRUE;
		props[nextProp].value =
		    bdfGetPropertyValue((char *)line + strlen(namebuf) + 1);
		if (!props[nextProp].value)
		    goto BAILOUT;
		break;
	    } else if (bdfIsInteger(secondbuf)) {
		stringProps[nextProp] = FALSE;
		props[nextProp].value = atoi(secondbuf);
		break;
	    } else {
		bdfError("invalid '%s' parameter value\n", namebuf);
		goto BAILOUT;
	    }

	case 3:
	    /*
	     * Possibilites include: valid quoted string with some white space
	     * invalid value (reject even if second string is integer)
	     */
	    if (secondbuf[0] == '"') {
		stringProps[nextProp] = TRUE;
		props[nextProp].value =
		    bdfGetPropertyValue((char *)line + strlen(namebuf) + 1);
		if (!props[nextProp].value)
		    goto BAILOUT;
		break;
	    } else {
		bdfError("invalid '%s' parameter value\n", namebuf);
		goto BAILOUT;
	    }
	}
	props[nextProp].name = bdfForceMakeAtom(namebuf, NULL);
	if (props[nextProp].name == None) {
	    bdfError("Empty property name.\n");
	    goto BAILOUT;
	}
	if (!bdfSpecialProperty(pFont, &props[nextProp],
				stringProps[nextProp], pState))
	    nextProp++;
    }

    line = bdfGetLine(file, lineBuf, BDFLINELEN);
    if (!line || !bdfIsPrefix(line, "ENDPROPERTIES")) {
	bdfError("missing 'ENDPROPERTIES'\n");
	goto BAILOUT;
    }
    if (!pState->haveFontAscent || !pState->haveFontDescent) {
	bdfError("missing 'FONT_ASCENT' or 'FONT_DESCENT' properties\n");
	goto BAILOUT;
    }
    if (bitmapFont->bitmapExtra) {
	bitmapFont->bitmapExtra->info.fontAscent = pFont->info.fontAscent;
	bitmapFont->bitmapExtra->info.fontDescent = pFont->info.fontDescent;
    }
    if (!pState->pointSizeProp) {
	props[nextProp].name = bdfForceMakeAtom("POINT_SIZE", NULL);
	props[nextProp].value = (INT32) (pState->pointSize * 10.0);
	stringProps[nextProp] = FALSE;
	pState->pointSizeProp = &props[nextProp];
	nextProp++;
    }
    if (!pState->fontProp) {
	props[nextProp].name = bdfForceMakeAtom("FONT", NULL);
	props[nextProp].value = (INT32) bdfForceMakeAtom(pState->fontName, NULL);
	stringProps[nextProp] = TRUE;
	pState->fontProp = &props[nextProp];
	nextProp++;
    }
    if (!pState->weightProp) {
	props[nextProp].name = bdfForceMakeAtom("WEIGHT", NULL);
	props[nextProp].value = -1;	/* computed later */
	stringProps[nextProp] = FALSE;
	pState->weightProp = &props[nextProp];
	nextProp++;
    }
    if (!pState->resolutionProp &&
	pState->resolution_x == pState->resolution_y) {
	props[nextProp].name = bdfForceMakeAtom("RESOLUTION", NULL);
	props[nextProp].value = (INT32) ((pState->resolution_x * 100.0) / 72.27);
	stringProps[nextProp] = FALSE;
	pState->resolutionProp = &props[nextProp];
	nextProp++;
    }
    if (!pState->resolutionXProp) {
	props[nextProp].name = bdfForceMakeAtom("RESOLUTION_X", NULL);
	props[nextProp].value = (INT32) pState->resolution_x;
	stringProps[nextProp] = FALSE;
	pState->resolutionProp = &props[nextProp];
	nextProp++;
    }
    if (!pState->resolutionYProp) {
	props[nextProp].name = bdfForceMakeAtom("RESOLUTION_Y", NULL);
	props[nextProp].value = (INT32) pState->resolution_y;
	stringProps[nextProp] = FALSE;
	pState->resolutionProp = &props[nextProp];
	nextProp++;
    }
    if (!pState->xHeightProp) {
	props[nextProp].name = bdfForceMakeAtom("X_HEIGHT", NULL);
	props[nextProp].value = -1;	/* computed later */
	stringProps[nextProp] = FALSE;
	pState->xHeightProp = &props[nextProp];
	nextProp++;
    }
    if (!pState->quadWidthProp) {
	props[nextProp].name = bdfForceMakeAtom("QUAD_WIDTH", NULL);
	props[nextProp].value = -1;	/* computed later */
	stringProps[nextProp] = FALSE;
	pState->quadWidthProp = &props[nextProp];
	nextProp++;
    }
    pFont->info.nprops = nextProp;
    return (TRUE);
BAILOUT:
    if (pFont->info.isStringProp) {
	free(pFont->info.isStringProp);
	pFont->info.isStringProp = NULL;
    }
    if (pFont->info.props) {
	free(pFont->info.props);
	pFont->info.props = NULL;
    }
    while (line && bdfIsPrefix(line, "ENDPROPERTIES"))
	line = bdfGetLine(file, lineBuf, BDFLINELEN);
    return (FALSE);
}
