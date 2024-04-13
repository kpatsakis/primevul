jbig2_decode_generic_template3(Jbig2Ctx *ctx,
                               Jbig2Segment *segment,
                               const Jbig2GenericRegionParams *params, Jbig2ArithState *as, Jbig2Image *image, Jbig2ArithCx *GB_stats)
{
    const int GBW = image->width;
    const int GBH = image->height;
    const int rowstride = image->stride;
    byte *gbreg_line = (byte *) image->data;
    int x, y;

    /* this routine only handles the nominal AT location */

#ifdef OUTPUT_PBM
    printf("P4\n%d %d\n", GBW, GBH);
#endif

    if (GBW <= 0)
                return 0;

    for (y = 0; y < GBH; y++) {
        uint32_t CONTEXT;
        uint32_t line_m1;
        int padded_width = (GBW + 7) & -8;

        line_m1 = (y >= 1) ? gbreg_line[-rowstride] : 0;
        CONTEXT = (line_m1 >> 1) & 0x3f0;

        /* 6.2.5.7 3d */
        for (x = 0; x < padded_width; x += 8) {
            byte result = 0;
            int x_minor;
            int minor_width = GBW - x > 8 ? 8 : GBW - x;

            if (y >= 1)
                line_m1 = (line_m1 << 8) | (x + 8 < GBW ? gbreg_line[-rowstride + (x >> 3) + 1] : 0);

            /* This is the speed-critical inner loop. */
            for (x_minor = 0; x_minor < minor_width; x_minor++) {
                bool bit;

                bit = jbig2_arith_decode(as, &GB_stats[CONTEXT]);
                if (bit < 0)
                            return -1;
                result |= bit << (7 - x_minor);
                CONTEXT = ((CONTEXT & 0x1f7) << 1) | bit | ((line_m1 >> (10 - x_minor)) & 0x010);
            }
            gbreg_line[x >> 3] = result;
        }
#ifdef OUTPUT_PBM
        fwrite(gbreg_line, 1, rowstride, stdout);
#endif
        gbreg_line += rowstride;
    }

    return 0;
}
