CatalogueListNextFontWithInfo(pointer client, FontPathElementPtr fpe, 
			      char **namep, int *namelenp, 
			      FontInfoPtr *pFontInfo,
			      int *numFonts, pointer private)
{
    LFWIDataPtr		data = private;
    CataloguePtr	cat = fpe->private;
    int			ret;

    if (data->current == cat->fpeCount)
    {
	xfree(data);
	return BadFontName;
    }

    ret = FontFileListNextFontWithInfo(client, cat->fpeList[data->current],
				       namep, namelenp,
				       pFontInfo, numFonts,
				       data->privates[data->current]);
    if (ret == BadFontName)
    {
	data->current++;
	return CatalogueListNextFontWithInfo(client, fpe, namep, namelenp,
					     pFontInfo, numFonts, private);
    }

    return ret;
}
