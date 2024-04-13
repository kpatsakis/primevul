jbig2_build_huffman_table(Jbig2Ctx *ctx, const Jbig2HuffmanParams *params)
{
    int *LENCOUNT;
    int LENMAX = -1;
    const int lencountcount = 256;
    const Jbig2HuffmanLine *lines = params->lines;
    int n_lines = params->n_lines;
    int i, j;
    int max_j;
    int log_table_size = 0;
    Jbig2HuffmanTable *result;
    Jbig2HuffmanEntry *entries;
    int CURLEN;
    int firstcode = 0;
    int CURCODE;
    int CURTEMP;

    LENCOUNT = jbig2_new(ctx, int, lencountcount);

    if (LENCOUNT == NULL) {
        jbig2_error(ctx, JBIG2_SEVERITY_FATAL, -1, "couldn't allocate storage for huffman histogram");
        return NULL;
    }
    memset(LENCOUNT, 0, sizeof(int) * lencountcount);

    /* B.3, 1. */
    for (i = 0; i < params->n_lines; i++) {
        int PREFLEN = lines[i].PREFLEN;
        int lts;

        if (PREFLEN > LENMAX) {
            for (j = LENMAX + 1; j < PREFLEN + 1; j++)
                LENCOUNT[j] = 0;
            LENMAX = PREFLEN;
        }
        LENCOUNT[PREFLEN]++;

        lts = PREFLEN + lines[i].RANGELEN;
        if (lts > LOG_TABLE_SIZE_MAX)
            lts = PREFLEN;
        if (lts <= LOG_TABLE_SIZE_MAX && log_table_size < lts)
            log_table_size = lts;
    }
    jbig2_error(ctx, JBIG2_SEVERITY_DEBUG, -1, "constructing huffman table log size %d", log_table_size);
    max_j = 1 << log_table_size;

    result = jbig2_new(ctx, Jbig2HuffmanTable, 1);
    if (result == NULL) {
        jbig2_error(ctx, JBIG2_SEVERITY_FATAL, -1, "couldn't allocate result storage in jbig2_build_huffman_table");
        jbig2_free(ctx->allocator, LENCOUNT);
        return NULL;
    }
    result->log_table_size = log_table_size;
    entries = jbig2_new(ctx, Jbig2HuffmanEntry, max_j);
    if (entries == NULL) {
        jbig2_error(ctx, JBIG2_SEVERITY_FATAL, -1, "couldn't allocate entries storage in jbig2_build_huffman_table");
        jbig2_free(ctx->allocator, result);
        jbig2_free(ctx->allocator, LENCOUNT);
        return NULL;
    }
    /* fill now to catch missing JBIG2Globals later */
    memset(entries, 0xFF, sizeof(Jbig2HuffmanEntry) * max_j);
    result->entries = entries;

    LENCOUNT[0] = 0;

    for (CURLEN = 1; CURLEN <= LENMAX; CURLEN++) {
        int shift = log_table_size - CURLEN;

        /* B.3 3.(a) */
        firstcode = (firstcode + LENCOUNT[CURLEN - 1]) << 1;
        CURCODE = firstcode;
        /* B.3 3.(b) */
        for (CURTEMP = 0; CURTEMP < n_lines; CURTEMP++) {
            int PREFLEN = lines[CURTEMP].PREFLEN;

            if (PREFLEN == CURLEN) {
                int RANGELEN = lines[CURTEMP].RANGELEN;
                int start_j = CURCODE << shift;
                int end_j = (CURCODE + 1) << shift;
                byte eflags = 0;

                if (end_j > max_j) {
                    jbig2_error(ctx, JBIG2_SEVERITY_FATAL, -1, "ran off the end of the entries table! (%d >= %d)", end_j, max_j);
                    jbig2_free(ctx->allocator, result->entries);
                    jbig2_free(ctx->allocator, result);
                    jbig2_free(ctx->allocator, LENCOUNT);
                    return NULL;
                }
                /* todo: build extension tables */
                if (params->HTOOB && CURTEMP == n_lines - 1)
                    eflags |= JBIG2_HUFFMAN_FLAGS_ISOOB;
                if (CURTEMP == n_lines - (params->HTOOB ? 3 : 2))
                    eflags |= JBIG2_HUFFMAN_FLAGS_ISLOW;
                if (PREFLEN + RANGELEN > LOG_TABLE_SIZE_MAX) {
                    for (j = start_j; j < end_j; j++) {
                        entries[j].u.RANGELOW = lines[CURTEMP].RANGELOW;
                        entries[j].PREFLEN = PREFLEN;
                        entries[j].RANGELEN = RANGELEN;
                        entries[j].flags = eflags;
                    }
                } else {
                    for (j = start_j; j < end_j; j++) {
                        int32_t HTOFFSET = (j >> (shift - RANGELEN)) & ((1 << RANGELEN) - 1);

                        if (eflags & JBIG2_HUFFMAN_FLAGS_ISLOW)
                            entries[j].u.RANGELOW = lines[CURTEMP].RANGELOW - HTOFFSET;
                        else
                            entries[j].u.RANGELOW = lines[CURTEMP].RANGELOW + HTOFFSET;
                        entries[j].PREFLEN = PREFLEN + RANGELEN;
                        entries[j].RANGELEN = 0;
                        entries[j].flags = eflags;
                    }
                }
                CURCODE++;
            }
        }
    }

    jbig2_free(ctx->allocator, LENCOUNT);

    return result;
}
