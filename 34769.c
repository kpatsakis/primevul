CatalogueStartListFonts(pointer client, FontPathElementPtr fpe, 
			char *pat, int len, int max, pointer *privatep,
			int mark_aliases)
{
    CataloguePtr	cat = fpe->private;
    LFWIDataPtr		data;
    int			ret, i, j;

    CatalogueRescan (fpe);

    data = (LFWIDataPtr) xalloc (sizeof *data +
				 sizeof data->privates[0] * cat->fpeCount);
    if (!data)
	return AllocError;
    data->privates = (pointer *) (data + 1);

    for (i = 0; i < cat->fpeCount; i++)
    {
	ret = FontFileStartListFonts(client, cat->fpeList[i], pat, len,
				     max, &data->privates[i], mark_aliases);
	if (ret != Successful)
	    goto bail;
    }

    data->current = 0;
    *privatep = (pointer) data;
    return Successful;

 bail:
    for (j = 0; j < i; j++)
	/* FIXME: we have no way to free the per-fpe privates. */;
    xfree (data);

    return AllocError;
}
