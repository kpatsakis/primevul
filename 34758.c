CatalogueAddFPE (CataloguePtr cat, FontPathElementPtr fpe)
{
    FontPathElementPtr *new;

    if (cat->fpeCount >= cat->fpeAlloc)
    {
	if (cat->fpeAlloc == 0)
	    cat->fpeAlloc = 16;
	else
	    cat->fpeAlloc *= 2;
	
	new = xrealloc(cat->fpeList,
		       cat->fpeAlloc * sizeof(FontPathElementPtr));
	if (new == NULL)
	    return AllocError;

	cat->fpeList = new;
    }

    cat->fpeList[cat->fpeCount++] = fpe;

    return Successful;
}
