static int setdevicepspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    int code = 0;
    gs_color_space *pcs;
    ref bpp;

    /* The comment in the original PostScript (gs_lev2.ps) said
     * "DevicePixel is actually a LanguageLevel 3 feature; it is here for
     *  historical reasons." Actually DevicePixel is a Display PostScript
     * space, as far as I can tell. It certainly isn't a level 3 space.
     * Preserve the old behaviour anyway.
     */
    if (i_ctx_p->language_level < 2)
        return_error(gs_error_undefined);

    *cont = 0;
    code = array_get(imemory, r, 1, &bpp);
    if (code < 0)
        return code;
    if (!r_has_type(&bpp, t_integer))
        return_error(gs_error_typecheck);
    code = gs_cspace_new_DevicePixel(imemory, &pcs, (int)bpp.value.intval);
    if (code < 0)
        return code;
    code = gs_setcolorspace(igs, pcs);
    /* release reference from construction */
    *stage = 0;
    rc_decrement_only_cs(pcs, "setseparationspace");
    return code;
}
