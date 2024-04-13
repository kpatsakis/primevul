jbig2_table(Jbig2Ctx *ctx, Jbig2Segment *segment, const byte *segment_data)
{
    Jbig2HuffmanParams *params = NULL;
    Jbig2HuffmanLine *line = NULL;

    segment->result = NULL;
    if (segment->data_length < 10)
        goto too_short;

    {
        /* B.2 1) (B.2.1) Code table flags */
        const int code_table_flags = segment_data[0];
        const int HTOOB = code_table_flags & 0x01;      /* Bit 0: HTOOB */

        /* Bits 1-3: Number of bits used in code table line prefix size fields */
        const int HTPS = (code_table_flags >> 1 & 0x07) + 1;

        /* Bits 4-6: Number of bits used in code table line range size fields */
        const int HTRS = (code_table_flags >> 4 & 0x07) + 1;

        /* B.2 2) (B.2.2) The lower bound of the first table line in the encoded table */
        const int32_t HTLOW = jbig2_get_int32(segment_data + 1);

        /* B.2 3) (B.2.3) One larger than the upeer bound of
           the last normal table line in the encoded table */
        const int32_t HTHIGH = jbig2_get_int32(segment_data + 5);

        /* estimated number of lines int this table, used for alloacting memory for lines */
        const size_t lines_max = (segment->data_length * 8 - HTPS * (HTOOB ? 3 : 2)) / (HTPS + HTRS) + (HTOOB ? 3 : 2);

        /* points to a first table line data */
        const byte *lines_data = segment_data + 9;
        const size_t lines_data_bitlen = (segment->data_length - 9) * 8;        /* length in bit */

        /* bit offset: controls bit reading */
        size_t boffset = 0;

        /* B.2 4) */
        int32_t CURRANGELOW = HTLOW;
        size_t NTEMP = 0;

#ifdef JBIG2_DEBUG
        jbig2_error(ctx, JBIG2_SEVERITY_DEBUG, segment->number,
                    "DECODING USER TABLE... Flags: %d, HTOOB: %d, HTPS: %d, HTRS: %d, HTLOW: %d, HTHIGH: %d",
                    code_table_flags, HTOOB, HTPS, HTRS, HTLOW, HTHIGH);
#endif

        /* allocate HuffmanParams & HuffmanLine */
        params = jbig2_new(ctx, Jbig2HuffmanParams, 1);
        if (params == NULL) {
            jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number, "Could not allocate Huffman Table Parameter");
            goto error_exit;
        }
        line = jbig2_new(ctx, Jbig2HuffmanLine, lines_max);
        if (line == NULL) {
            jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number, "Could not allocate Huffman Table Lines");
            goto error_exit;
        }
        /* B.2 5) */
        while (CURRANGELOW < HTHIGH) {
            /* B.2 5) a) */
            if (boffset + HTPS >= lines_data_bitlen)
                goto too_short;
            line[NTEMP].PREFLEN = jbig2_table_read_bits(lines_data, &boffset, HTPS);
            /* B.2 5) b) */
            if (boffset + HTRS >= lines_data_bitlen)
                goto too_short;
            line[NTEMP].RANGELEN = jbig2_table_read_bits(lines_data, &boffset, HTRS);
            /* B.2 5) c) */
            line[NTEMP].RANGELOW = CURRANGELOW;
            CURRANGELOW += (1 << line[NTEMP].RANGELEN);
            NTEMP++;
        }
        /* B.2 6), B.2 7) lower range table line */
        if (boffset + HTPS >= lines_data_bitlen)
            goto too_short;
        line[NTEMP].PREFLEN = jbig2_table_read_bits(lines_data, &boffset, HTPS);
        line[NTEMP].RANGELEN = 32;
        line[NTEMP].RANGELOW = HTLOW - 1;
        NTEMP++;
        /* B.2 8), B.2 9) upper range table line */
        if (boffset + HTPS >= lines_data_bitlen)
            goto too_short;
        line[NTEMP].PREFLEN = jbig2_table_read_bits(lines_data, &boffset, HTPS);
        line[NTEMP].RANGELEN = 32;
        line[NTEMP].RANGELOW = HTHIGH;
        NTEMP++;
        /* B.2 10) */
        if (HTOOB) {
            /* B.2 10) a), B.2 10) b) out-of-bound table line */
            if (boffset + HTPS >= lines_data_bitlen)
                goto too_short;
            line[NTEMP].PREFLEN = jbig2_table_read_bits(lines_data, &boffset, HTPS);
            line[NTEMP].RANGELEN = 0;
            line[NTEMP].RANGELOW = 0;
            NTEMP++;
        }
        if (NTEMP != lines_max) {
            Jbig2HuffmanLine *new_line = jbig2_renew(ctx, line,
                                         Jbig2HuffmanLine, NTEMP);

            if (new_line == NULL) {
                jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number, "Could not reallocate Huffman Table Lines");
                goto error_exit;
            }
            line = new_line;
        }
        params->HTOOB = HTOOB;
        params->n_lines = NTEMP;
        params->lines = line;
        segment->result = params;

#ifdef JBIG2_DEBUG
        {
            int i;

            for (i = 0; i < NTEMP; i++) {
                jbig2_error(ctx, JBIG2_SEVERITY_DEBUG, segment->number,
                            "Line: %d, PREFLEN: %d, RANGELEN: %d, RANGELOW: %d",
                            i, params->lines[i].PREFLEN, params->lines[i].RANGELEN, params->lines[i].RANGELOW);
            }
        }
#endif
    }
    return 0;

too_short:
    jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number, "Segment too short");
error_exit:
    if (line != NULL) {
        jbig2_free(ctx->allocator, line);
    }
    if (params != NULL) {
        jbig2_free(ctx->allocator, params);
    }
    return -1;
}
