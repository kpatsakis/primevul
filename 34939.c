dump_page_info(Jbig2Ctx *ctx, Jbig2Segment *segment, Jbig2Page *page)
{
    if (page->x_resolution == 0) {
        jbig2_error(ctx, JBIG2_SEVERITY_INFO, segment->number, "page %d image is %dx%d (unknown res)", page->number, page->width, page->height);
    } else if (page->x_resolution == page->y_resolution) {
        jbig2_error(ctx, JBIG2_SEVERITY_INFO, segment->number, "page %d image is %dx%d (%d ppm)", page->number, page->width, page->height, page->x_resolution);
    } else {
        jbig2_error(ctx, JBIG2_SEVERITY_INFO, segment->number,
                    "page %d image is %dx%d (%dx%d ppm)", page->number, page->width, page->height, page->x_resolution, page->y_resolution);
    }
    if (page->striped) {
        jbig2_error(ctx, JBIG2_SEVERITY_INFO, segment->number, "\tmaximum stripe size: %d", page->stripe_size);
    }
}
