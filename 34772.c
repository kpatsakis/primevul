CatalogueUnrefFPEs (FontPathElementPtr fpe)
{
    CataloguePtr	cat = fpe->private;
    FontPathElementPtr	subfpe;
    int			i;

    for (i = 0; i < cat->fpeCount; i++)
    {
	subfpe = cat->fpeList[i];
	subfpe->refcount--;
	if (subfpe->refcount == 0)
	{
	    FontFileFreeFPE (subfpe);
	    xfree(subfpe->name);
	    xfree(subfpe);
	}
    }

    cat->fpeCount = 0;
}
