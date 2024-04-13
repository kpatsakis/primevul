CatalogueListFonts (pointer client, FontPathElementPtr fpe, char *pat, 
		    int len, int max, FontNamesPtr names)
{
    CataloguePtr cat = fpe->private;
    FontPathElementPtr subfpe;
    FontDirectoryPtr dir;
    int i;

    CatalogueRescan (fpe);

    for (i = 0; i < cat->fpeCount; i++)
    {
	subfpe = cat->fpeList[i];
	dir = subfpe->private;
	FontFileListFonts(client, subfpe, pat, len, max, names);
    }

    return Successful;
}
