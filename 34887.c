jbig2_data_in(Jbig2Ctx *ctx, const unsigned char *data, size_t size)
{
    const size_t initial_buf_size = 1024;

    if (ctx->buf == NULL) {
        size_t buf_size = initial_buf_size;

        do
            buf_size <<= 1;
        while (buf_size < size);
        ctx->buf = jbig2_new(ctx, byte, buf_size);
        if (ctx->buf == NULL) {
            return jbig2_error(ctx, JBIG2_SEVERITY_FATAL, -1, "failed to allocate ctx->buf in jbig2_data_in");
        }
        ctx->buf_size = buf_size;
        ctx->buf_rd_ix = 0;
        ctx->buf_wr_ix = 0;
    } else if (ctx->buf_wr_ix + size > ctx->buf_size) {
        if (ctx->buf_rd_ix <= (ctx->buf_size >> 1) && ctx->buf_wr_ix - ctx->buf_rd_ix + size <= ctx->buf_size) {
            memmove(ctx->buf, ctx->buf + ctx->buf_rd_ix, ctx->buf_wr_ix - ctx->buf_rd_ix);
        } else {
            byte *buf;
            size_t buf_size = initial_buf_size;

            do
                buf_size <<= 1;
            while (buf_size < ctx->buf_wr_ix - ctx->buf_rd_ix + size);
            buf = jbig2_new(ctx, byte, buf_size);
            if (buf == NULL) {
                return jbig2_error(ctx, JBIG2_SEVERITY_FATAL, -1, "failed to allocate buf in jbig2_data_in");
            }
            memcpy(buf, ctx->buf + ctx->buf_rd_ix, ctx->buf_wr_ix - ctx->buf_rd_ix);
            jbig2_free(ctx->allocator, ctx->buf);
            ctx->buf = buf;
            ctx->buf_size = buf_size;
        }
        ctx->buf_wr_ix -= ctx->buf_rd_ix;
        ctx->buf_rd_ix = 0;
    }
    memcpy(ctx->buf + ctx->buf_wr_ix, data, size);
    ctx->buf_wr_ix += size;

    /* data has now been added to buffer */

    for (;;) {
        const byte jbig2_id_string[8] = { 0x97, 0x4a, 0x42, 0x32, 0x0d, 0x0a, 0x1a, 0x0a };
        Jbig2Segment *segment;
        size_t header_size;
        int code;

        switch (ctx->state) {
        case JBIG2_FILE_HEADER:
            /* D.4.1 */
            if (ctx->buf_wr_ix - ctx->buf_rd_ix < 9)
                return 0;
            if (memcmp(ctx->buf + ctx->buf_rd_ix, jbig2_id_string, 8))
                return jbig2_error(ctx, JBIG2_SEVERITY_FATAL, -1, "Not a JBIG2 file header");
            /* D.4.2 */
            ctx->file_header_flags = ctx->buf[ctx->buf_rd_ix + 8];
            if (ctx->file_header_flags & 0xFC) {
                jbig2_error(ctx, JBIG2_SEVERITY_WARNING, -1, "reserved bits (2-7) of file header flags are not zero (0x%02x)", ctx->file_header_flags);
            }
            /* D.4.3 */
            if (!(ctx->file_header_flags & 2)) {        /* number of pages is known */
                if (ctx->buf_wr_ix - ctx->buf_rd_ix < 13)
                    return 0;
                ctx->n_pages = jbig2_get_uint32(ctx->buf + ctx->buf_rd_ix + 9);
                ctx->buf_rd_ix += 13;
                if (ctx->n_pages == 1)
                    jbig2_error(ctx, JBIG2_SEVERITY_INFO, -1, "file header indicates a single page document");
                else
                    jbig2_error(ctx, JBIG2_SEVERITY_INFO, -1, "file header indicates a %d page document", ctx->n_pages);
            } else {            /* number of pages not known */

                ctx->n_pages = 0;
                ctx->buf_rd_ix += 9;
            }
            /* determine the file organization based on the flags - D.4.2 again */
            if (ctx->file_header_flags & 1) {
                ctx->state = JBIG2_FILE_SEQUENTIAL_HEADER;
                jbig2_error(ctx, JBIG2_SEVERITY_DEBUG, -1, "file header indicates sequential organization");
            } else {
                ctx->state = JBIG2_FILE_RANDOM_HEADERS;
                jbig2_error(ctx, JBIG2_SEVERITY_DEBUG, -1, "file header indicates random-access organization");

            }
            break;
        case JBIG2_FILE_SEQUENTIAL_HEADER:
        case JBIG2_FILE_RANDOM_HEADERS:
            segment = jbig2_parse_segment_header(ctx, ctx->buf + ctx->buf_rd_ix, ctx->buf_wr_ix - ctx->buf_rd_ix, &header_size);
            if (segment == NULL)
                return 0;       /* need more data */
            ctx->buf_rd_ix += header_size;

            if (ctx->n_segments == ctx->n_segments_max)
                ctx->segments = jbig2_renew(ctx, ctx->segments, Jbig2Segment *, (ctx->n_segments_max <<= 2));

            ctx->segments[ctx->n_segments++] = segment;
            if (ctx->state == JBIG2_FILE_RANDOM_HEADERS) {
                if ((segment->flags & 63) == 51)        /* end of file */
                    ctx->state = JBIG2_FILE_RANDOM_BODIES;
            } else              /* JBIG2_FILE_SEQUENTIAL_HEADER */
                ctx->state = JBIG2_FILE_SEQUENTIAL_BODY;
            break;
        case JBIG2_FILE_SEQUENTIAL_BODY:
        case JBIG2_FILE_RANDOM_BODIES:
            segment = ctx->segments[ctx->segment_index];
            if (segment->data_length > ctx->buf_wr_ix - ctx->buf_rd_ix)
                return 0;       /* need more data */
            code = jbig2_parse_segment(ctx, segment, ctx->buf + ctx->buf_rd_ix);
            ctx->buf_rd_ix += segment->data_length;
            ctx->segment_index++;
            if (ctx->state == JBIG2_FILE_RANDOM_BODIES) {
                if (ctx->segment_index == ctx->n_segments)
                    ctx->state = JBIG2_FILE_EOF;
            } else {            /* JBIG2_FILE_SEQUENCIAL_BODY */

                ctx->state = JBIG2_FILE_SEQUENTIAL_HEADER;
            }
            if (code < 0) {
                ctx->state = JBIG2_FILE_EOF;
                return code;
            }
            break;
        case JBIG2_FILE_EOF:
            if (ctx->buf_rd_ix == ctx->buf_wr_ix)
                return 0;
            return jbig2_error(ctx, JBIG2_SEVERITY_WARNING, -1, "Garbage beyond end of file");
        }
    }
}
