av_cold void ff_pngdsp_init(PNGDSPContext *dsp)
{
    dsp->add_bytes_l2         = add_bytes_l2_c;
    dsp->add_paeth_prediction = ff_add_png_paeth_prediction;

    if (ARCH_X86) ff_pngdsp_init_x86(dsp);
}
