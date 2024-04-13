jbig2_huffman_get(Jbig2HuffmanState *hs, const Jbig2HuffmanTable *table, bool *oob)
{
    Jbig2HuffmanEntry *entry;
    byte flags;
    int offset_bits = hs->offset_bits;
    uint32_t this_word = hs->this_word;
    uint32_t next_word;
    int RANGELEN;
    int32_t result;

    if (hs->offset_limit && hs->offset >= hs->offset_limit) {
        jbig2_error(hs->ctx, JBIG2_SEVERITY_FATAL, -1, "end of Jbig2WordStream reached at offset %d", hs->offset);
        if (oob)
            *oob = -1;
        return -1;
    }

    for (;;) {
        int log_table_size = table->log_table_size;
        int PREFLEN;

        /* SumatraPDF: shifting by the size of the operand is undefined */
        entry = &table->entries[log_table_size > 0 ? this_word >> (32 - log_table_size) : 0];
        flags = entry->flags;
        PREFLEN = entry->PREFLEN;
        if ((flags == (byte) - 1) && (PREFLEN == (byte) - 1) && (entry->u.RANGELOW == -1)) {
            if (oob)
                *oob = -1;
            return -1;
        }

        next_word = hs->next_word;
        offset_bits += PREFLEN;
        if (offset_bits >= 32) {
            this_word = next_word;
            hs->offset += 4;
            next_word = huff_get_next_word(hs, hs->offset + 4);
            offset_bits -= 32;
            hs->next_word = next_word;
            PREFLEN = offset_bits;
        }
        if (PREFLEN)
            this_word = (this_word << PREFLEN) | (next_word >> (32 - offset_bits));
        if (flags & JBIG2_HUFFMAN_FLAGS_ISEXT) {
            table = entry->u.ext_table;
        } else
            break;
    }
    result = entry->u.RANGELOW;
    RANGELEN = entry->RANGELEN;
    if (RANGELEN > 0) {
        int32_t HTOFFSET;

        HTOFFSET = this_word >> (32 - RANGELEN);
        if (flags & JBIG2_HUFFMAN_FLAGS_ISLOW)
            result -= HTOFFSET;
        else
            result += HTOFFSET;

        offset_bits += RANGELEN;
        if (offset_bits >= 32) {
            this_word = next_word;
            hs->offset += 4;
            next_word = huff_get_next_word(hs, hs->offset + 4);
            offset_bits -= 32;
            hs->next_word = next_word;
            RANGELEN = offset_bits;
        }
        if (RANGELEN)
            this_word = (this_word << RANGELEN) | (next_word >> (32 - offset_bits));
    }

    hs->this_word = this_word;
    hs->offset_bits = offset_bits;

    if (oob != NULL)
        *oob = (flags & JBIG2_HUFFMAN_FLAGS_ISOOB);

    return result;
}
