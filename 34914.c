jbig2_generic_stats_size(Jbig2Ctx *ctx, int template)
{
    int stats_size = template == 0 ? 1 << 16 : template == 1 ? 1 << 1 << 13 : 1 << 10;

                                        return stats_size;
}
