static int setcmykspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    os_ptr op = osp;
    gs_color_space  *pcs;
    int code=0;
    ref stref;

    do {
        switch (*stage) {
            case 0:
                if (istate->use_cie_color.value.boolval && !CIESubst) {
                    byte *body;
                    ref *nosubst;

                    code = dict_find_string(systemdict, "NOSUBSTDEVICECOLORS", &nosubst);
                    if (code != 0) {
                        if (!r_has_type(nosubst, t_boolean))
                            return_error(gs_error_typecheck);
                    }
                    if (code != 0 && nosubst->value.boolval) {
                        *stage = 4;
                        *cont = 1;
                        body = ialloc_string(32, "string");
                        if (body == 0)
                            return_error(gs_error_VMerror);
                        memcpy(body, "/DefaultCMYK ..nosubstdevicetest",32);
                        make_string(&stref, a_all | icurrent_space, 32, body);
                        r_set_attrs(&stref, a_executable);
                        esp++;
                        ref_assign(esp, &stref);
                        return o_push_estack;
                    } else {
                        *stage = 2;
                        *cont = 1;
                        body = ialloc_string(47, "string");
                        if (body == 0)
                            return_error(gs_error_VMerror);
                        memcpy(body, "{/DefaultCMYK /ColorSpace findresource} stopped", 47);
                        make_string(&stref, a_all | icurrent_space, 47, body);
                        r_set_attrs(&stref, a_executable);
                        esp++;
                        ref_assign(esp, &stref);
                        return o_push_estack;
                    }
                }
                /* fall through */
            case 1:
                pcs = gs_cspace_new_DeviceCMYK(imemory);
                if (pcs == NULL)
                    return_error(gs_error_VMerror);
                code = gs_setcolorspace(igs, pcs);
                if (code >= 0) {
                    gs_client_color *pcc = gs_currentcolor_inline(igs);

                    cs_adjust_color_count(igs, -1); /* not strictly necessary */
                    pcc->paint.values[0] = 0;
                    pcc->paint.values[1] = 0;
                    pcc->paint.values[2] = 0;
                    pcc->paint.values[3] = 1;
                    pcc->pattern = 0;		/* for GC */
                    gx_unset_dev_color(igs);
                }
                rc_decrement_only_cs(pcs, "zsetdevcspace");
                *cont = 0;
                *stage = 0;
                break;
            case 2:
                if (!r_has_type(op, t_boolean))
                    return_error(gs_error_typecheck);
                if (op->value.boolval) {
                    /* Failed to find the /DefaultCMYK CSA, so give up and
                     * just use DeviceCMYK
                     */
                    pop(1);
                    *stage = 1;
                    break;
                }
                pop(1);
                *stage = 3;
                code = setcolorspace_nosubst(i_ctx_p);
                if (code != 0)
                    return code;
                break;
            case 3:
                /* We end up here after setting the DefaultGray CIE space
                 * We've finished setting the gray color space, so we
                 * just exit now
                 */
                *cont = 0;
                *stage = 0;
                break;
            case 4:
                /* We come here if /UseCIEColor is true, and NOSUBSTDEVICECOLORS
                 * is also true. We will have a boolean on the stack, if its true
                 * then we need to set the space (also on the stack), invoke
                 * .includecolorspace, and set /DeviceGray, otherwise we just need
                 * to set DeviceGray. See gs-cspace.ps.
                 */
                if (!r_has_type(op, t_boolean))
                    return_error(gs_error_typecheck);
                pop(1);
                *stage = 1;
                *cont = 1;
                if (op->value.boolval) {
                    *stage = 5;
                    code = setcolorspace_nosubst(i_ctx_p);
                    if (code != 0)
                        return code;
                }
                break;
            case 5:
                /* After stage 4 above, if we had to set a color space, we come
                 * here. Now we need to use .includecolorspace to register the space
                 * with any high-level devices which want it.
                 */
                *stage = 1;
                *cont = 1;
                code = zincludecolorspace(i_ctx_p);
                if (code != 0)
                    return code;
                break;
        }
    } while (*stage);
    return code;
}
