jbig2_page_out(Jbig2Ctx *ctx)
{
    int index;

    /* search for a completed page */
    for (index = 0; index < ctx->max_page_index; index++) {
        if (ctx->pages[index].state == JBIG2_PAGE_COMPLETE) {
            Jbig2Image *img = ctx->pages[index].image;
            uint32_t page_number = ctx->pages[index].number;

            ctx->pages[index].state = JBIG2_PAGE_RETURNED;
            if (img != NULL) {
                jbig2_error(ctx, JBIG2_SEVERITY_DEBUG, -1, "page %d returned to the client", page_number);
                return jbig2_image_clone(ctx, img);
            } else {
                jbig2_error(ctx, JBIG2_SEVERITY_WARNING, -1, "page %d returned with no associated image", page_number);
                ;               /* continue */
            }
        }
    }

    /* no pages available */
    return NULL;
}
