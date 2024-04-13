CatalogueStartListFontsWithInfo(pointer client, FontPathElementPtr fpe, 
				char *pat, int len, int max, 
				pointer *privatep)
{
    return CatalogueStartListFonts(client, fpe, pat, len, max, privatep, 0);
}
