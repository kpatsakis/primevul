map_components_to_colorants(const frac * pcc,
        const gs_devicen_color_map * pcolor_component_map, frac * plist)
{
    int i = pcolor_component_map->num_colorants - 1;
    int pos;

    /* Clear all output colorants first */
    for (; i >= 0; i--) {
        plist[i] = frac_0;
    }
    /* Map color components into output list */
    for (i = pcolor_component_map->num_components - 1; i >= 0; i--) {
        pos = pcolor_component_map->color_map[i];
        if (pos >= 0)
            plist[pos] = pcc[i];
    }
}
