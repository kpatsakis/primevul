CatalogueCloseFont (FontPathElementPtr fpe, FontPtr pFont)
{
    /* Note: this gets called with the actual subfpe where we found
     * the font, not the catalogue fpe. */

    FontFileCloseFont(fpe, pFont);
}
