CatalogueListNextFontOrAlias(pointer client, FontPathElementPtr fpe, 
			     char **namep, int *namelenp, char **resolvedp,
			     int *resolvedlenp, pointer private)
{
    LFWIDataPtr		data = private;
    CataloguePtr	cat = fpe->private;
    int			ret;

    if (data->current == cat->fpeCount)
    {
	xfree(data);
	return BadFontName;
    }

    ret = FontFileListNextFontOrAlias(client, cat->fpeList[data->current],
				      namep, namelenp,
				      resolvedp, resolvedlenp,
				      data->privates[data->current]);
    if (ret == BadFontName)
    {
	data->current++;
	return CatalogueListNextFontOrAlias(client, fpe, namep, namelenp,
					    resolvedp, resolvedlenp, private);
    }

    return ret;
}
