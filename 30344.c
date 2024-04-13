zswapcolors(i_ctx_t * i_ctx_p)
{
    ref_colorspace tmp_cs;
    ref            tmp_pat;

    tmp_cs                = istate->colorspace[0];
    istate->colorspace[0] = istate->colorspace[1];
    istate->colorspace[1] = tmp_cs;

    tmp_pat            = istate->pattern[0];
    istate->pattern[0] = istate->pattern[1];
    istate->pattern[1] = tmp_pat;

    return gs_swapcolors(igs);
}
