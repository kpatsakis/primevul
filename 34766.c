CatalogueOpenFont (pointer client, FontPathElementPtr fpe, Mask flags, 
		   char *name, int namelen, 
		   fsBitmapFormat format, fsBitmapFormatMask fmask,
		   XID id, FontPtr *pFont, char **aliasName, 
		   FontPtr non_cachable_font)
{
    CataloguePtr cat = fpe->private;
    FontPathElementPtr subfpe;
    FontDirectoryPtr dir;
    int i, status;

    CatalogueRescan (fpe);

    for (i = 0; i < cat->fpeCount; i++)
    {
	subfpe = cat->fpeList[i];
	dir = subfpe->private;
	status = FontFileOpenFont(client, subfpe, flags,
				  name, namelen, format, fmask, id,
				  pFont, aliasName, non_cachable_font);
	if (status == Successful || status == FontNameAlias)
	    return status;
    }
	    
    return BadFontName;
}
