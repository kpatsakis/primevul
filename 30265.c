static int is_same_colorspace(i_ctx_t * i_ctx_p, ref *space1, ref *space2, bool isCIE)
{
    PS_colour_space_t *oldcspace = 0, *newcspace = 0;
    ref oldspace, *poldspace = &oldspace, newspace, *pnewspace = &newspace;
    int code, CIESubst;

    /* Silence compiler warnings */
    oldspace.tas.type_attrs = 0;
    oldspace.tas.type_attrs = 0;

    ref_assign(pnewspace, space1);
    ref_assign(poldspace, space2);

    do {
        if (r_type(poldspace) != r_type(pnewspace))
            return 0;

        code = get_space_object(i_ctx_p, poldspace, &oldcspace);
        if (code < 0)
            return 0;

        code = get_space_object(i_ctx_p, pnewspace, &newcspace);
        if (code < 0)
            return 0;

        /* Check the two color space types are the same
         * (Indexed, Separation, DeviceCMYK etc).
         */
        if (strcmp(oldcspace->name, newcspace->name) != 0)
            return 0;

        /* Call the space-specific comparison routine */
        if (!oldcspace->compareproc(i_ctx_p, poldspace, pnewspace))
            return 0;

        /* See if current space is CIE based (which could happen
           if UseCIE had been true previously), but UseCIE is false
           and incoming space is device based.  This can occur
           when we are now processing a soft mask, which should not
           use the UseCIEColor option.

           Need to detect this case at both transitions

            Device Color UseCIEColor true
            Soft mask
                    Device color UseCIEColor false
            Soft mask
            Device color UseCIEColor true
            */

        if ( name_is_device_color(newcspace->name) ){
            if ( gs_color_space_is_CIE(gs_currentcolorspace_inline(i_ctx_p->pgs)) ){
                if ( !isCIE ) return 0; /*  The color spaces will be different */
            } else {
                if ( isCIE ) return 0; /*  The color spaces will be different */
            }
        }

        /* The current space is OK, if there is no alternate, then that's
         * good enough.
         */
        if (!oldcspace->alternateproc)
            break;

        /* Otherwise, retrieve the alternate space for each, and continue
         * round the loop, checking those.
         */
        code = oldcspace->alternateproc(i_ctx_p, poldspace, &poldspace, &CIESubst);
        if (code < 0)
            return 0;

        code = newcspace->alternateproc(i_ctx_p, pnewspace, &pnewspace, &CIESubst);
        if (code < 0)
            return 0;
    }
    while(1);

    return 1;
}
