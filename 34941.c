jbig2_page_info(Jbig2Ctx *ctx, Jbig2Segment *segment, const uint8_t *segment_data)
{
    Jbig2Page *page;

    /* a new page info segment implies the previous page is finished */
    page = &(ctx->pages[ctx->current_page]);
    if ((page->number != 0) && ((page->state == JBIG2_PAGE_NEW) || (page->state == JBIG2_PAGE_FREE))) {
        page->state = JBIG2_PAGE_COMPLETE;
        jbig2_error(ctx, JBIG2_SEVERITY_WARNING, segment->number, "unexpected page info segment, marking previous page finished");
    }

    /* find a free page */
    {
        int index, j;

        index = ctx->current_page;
        while (ctx->pages[index].state != JBIG2_PAGE_FREE) {
            index++;
            if (index >= ctx->max_page_index) {
                /* grow the list */
                ctx->pages = jbig2_renew(ctx, ctx->pages, Jbig2Page, (ctx->max_page_index <<= 2));
                for (j = index; j < ctx->max_page_index; j++) {
                    ctx->pages[j].state = JBIG2_PAGE_FREE;
                    ctx->pages[j].number = 0;
                    ctx->pages[j].image = NULL;

                }
            }
        }
        page = &(ctx->pages[index]);
        ctx->current_page = index;
        page->state = JBIG2_PAGE_NEW;
        page->number = segment->page_association;
    }

    /* FIXME: would be nice if we tried to work around this */
    if (segment->data_length < 19) {
        return jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number, "segment too short");
    }

    /* 7.4.8.x */
    page->width = jbig2_get_uint32(segment_data);
    page->height = jbig2_get_uint32(segment_data + 4);

    page->x_resolution = jbig2_get_uint32(segment_data + 8);
    page->y_resolution = jbig2_get_uint32(segment_data + 12);
    page->flags = segment_data[16];

    /* 7.4.8.6 */
    {
        int16_t striping = jbig2_get_int16(segment_data + 17);

        if (striping & 0x8000) {
            page->striped = TRUE;
            page->stripe_size = striping & 0x7FFF;
        } else {
            page->striped = FALSE;
            page->stripe_size = 0;      /* would page->height be better? */
        }
    }
    if (page->height == 0xFFFFFFFF && page->striped == FALSE) {
        jbig2_error(ctx, JBIG2_SEVERITY_WARNING, segment->number, "height is unspecified but page is not markes as striped");
        page->striped = TRUE;
    }
    page->end_row = 0;

    if (segment->data_length > 19) {
        jbig2_error(ctx, JBIG2_SEVERITY_WARNING, segment->number, "extra data in segment");
    }

    dump_page_info(ctx, segment, page);

    /* allocate an approprate page image buffer */
    /* 7.4.8.2 */
    if (page->height == 0xFFFFFFFF) {
        page->image = jbig2_image_new(ctx, page->width, page->stripe_size);
    } else {
        page->image = jbig2_image_new(ctx, page->width, page->height);
    }
    if (page->image == NULL) {
        return jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number, "failed to allocate buffer for page image");
    } else {
        /* 8.2 (3) fill the page with the default pixel value */
        jbig2_image_clear(ctx, page->image, (page->flags & 4));
        jbig2_error(ctx, JBIG2_SEVERITY_DEBUG, segment->number,
                    "allocated %dx%d page image (%d bytes)", page->image->width, page->image->height, page->image->stride * page->image->height);
    }

    return 0;
}
