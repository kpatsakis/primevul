jbig2_ctx_new(Jbig2Allocator *allocator, Jbig2Options options, Jbig2GlobalCtx *global_ctx, Jbig2ErrorCallback error_callback, void *error_callback_data)
{
    Jbig2Ctx *result;

    if (allocator == NULL)
        allocator = &jbig2_default_allocator;
    if (error_callback == NULL)
        error_callback = &jbig2_default_error;

    result = (Jbig2Ctx *) jbig2_alloc(allocator, sizeof(Jbig2Ctx), 1);
    if (result == NULL) {
        error_callback(error_callback_data, "initial context allocation failed!", JBIG2_SEVERITY_FATAL, -1);
        return result;
    }

    result->allocator = allocator;
    result->options = options;
    result->global_ctx = (const Jbig2Ctx *)global_ctx;
    result->error_callback = error_callback;
    result->error_callback_data = error_callback_data;

    result->state = (options & JBIG2_OPTIONS_EMBEDDED) ? JBIG2_FILE_SEQUENTIAL_HEADER : JBIG2_FILE_HEADER;

    result->buf = NULL;

    result->n_segments = 0;
    result->n_segments_max = 16;
    result->segments = jbig2_new(result, Jbig2Segment *, result->n_segments_max);
    if (result->segments == NULL) {
        error_callback(error_callback_data, "initial segments allocation failed!", JBIG2_SEVERITY_FATAL, -1);
        jbig2_free(allocator, result);
        return result;
    }
    result->segment_index = 0;

    result->current_page = 0;
    result->max_page_index = 4;
    result->pages = jbig2_new(result, Jbig2Page, result->max_page_index);
    if (result->pages == NULL) {
        error_callback(error_callback_data, "initial pages allocation failed!", JBIG2_SEVERITY_FATAL, -1);
        jbig2_free(allocator, result->segments);
        jbig2_free(allocator, result);
        return result;
    }
    {
        int index;

        for (index = 0; index < result->max_page_index; index++) {
            result->pages[index].state = JBIG2_PAGE_FREE;
            result->pages[index].number = 0;
            result->pages[index].image = NULL;
        }
    }

    return result;
}
