CatalogueFreeFPE (FontPathElementPtr fpe)
{
    CataloguePtr	cat = fpe->private;

    /* If the catalogue is modified while the xserver has fonts open
     * from the previous subfpes, we'll unref the old subfpes when we
     * reload the catalogue, and the xserver will the call FreeFPE on
     * them once it drops its last reference. Thus, the FreeFPE call
     * for the subfpe ends up here and we just forward it to
     * FontFileFreeFPE. */

    if (!CatalogueNameCheck (fpe->name))
	return FontFileFreeFPE (fpe);

    CatalogueUnrefFPEs (fpe);
    xfree(cat->fpeList);
    xfree(cat);

    return Successful;
}
