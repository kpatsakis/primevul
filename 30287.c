static int septransform(i_ctx_t *i_ctx_p, ref *sepspace, int *usealternate, int *stage, int *stack_depth)
{
    gx_device * dev = igs->device;
    ref sname, proc;
    int code, colorant_number;

    code = array_get(imemory, sepspace, 1, &sname);
    if (code < 0)
        return code;
    if (r_has_type(&sname, t_name)) {
        name_string_ref(imemory, &sname, &sname);
    }

    /* Check for /All and /None, never need the alternate for these */
    if (r_size(&sname) == 3 &&
        strncmp("All", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
        *usealternate = 0;
        return 0;
    }
    if (r_size(&sname) == 4 &&
        strncmp("None", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
        *usealternate = 0;
        return 0;
    }
    /*
     * Compare the colorant name to the device's.  If the device's
     * compare routine returns GX_DEVICE_COLOR_MAX_COMPONENTS then the
     * colorant is in the SeparationNames list but not in the
     * SeparationOrder list.
     */
    colorant_number = (*dev_proc(dev, get_color_comp_index))
                (dev, (const char *)sname.value.bytes, r_size(&sname), SEPARATION_NAME);
    if (colorant_number >= 0) {		/* If valid colorant name */
        *usealternate = 0;
    } else
        *usealternate = 1;

    if (*usealternate && *stage == 0) {
        (*stage)++;
        esp++;
        code = array_get(imemory, sepspace, 3, &proc);
        if (code < 0)
            return code;
        *esp = proc;
        return o_push_estack;
    }
    *stage = 0;
    return 0;
}
