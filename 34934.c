jbig2_image_free(Jbig2Ctx *ctx, Jbig2Image *image)
{
    if (image)
        jbig2_free(ctx->allocator, image->data);
    jbig2_free(ctx->allocator, image);
}
