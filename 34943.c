jbig2_release_page(Jbig2Ctx *ctx, Jbig2Image *image)
{
    int index;

    /* find the matching page struct and mark it released */
    for (index = 0; index < ctx->max_page_index; index++) {
        if (ctx->pages[index].image == image) {
            jbig2_image_release(ctx, image);
            ctx->pages[index].state = JBIG2_PAGE_RELEASED;
            jbig2_error(ctx, JBIG2_SEVERITY_DEBUG, -1, "page %d released by the client", ctx->pages[index].number);
            return 0;
        }
    }

    /* no matching pages */
    jbig2_error(ctx, JBIG2_SEVERITY_WARNING, -1, "jbig2_release_page called on unknown page");
    return 1;
}
