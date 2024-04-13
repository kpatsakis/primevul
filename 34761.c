CatalogueInitFPE (FontPathElementPtr fpe)
{
    CataloguePtr	cat;

    cat = (CataloguePtr) xalloc(sizeof *cat);
    if (cat == NULL)
	return AllocError;

    fpe->private = (pointer) cat;
    cat->fpeCount = 0;
    cat->fpeAlloc = 0;
    cat->fpeList = NULL;
    cat->mtime = 0;

    return CatalogueRescan (fpe);
}
