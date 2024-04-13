static void blitter_set_dst_dim(struct vrend_blitter_ctx *blit_ctx,
                                unsigned width, unsigned height)
{
   blit_ctx->dst_width = width;
   blit_ctx->dst_height = height;
}
