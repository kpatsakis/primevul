jbig2_dump_huffman_table(const Jbig2HuffmanTable *table)
{
    int i;
    int table_size = (1 << table->log_table_size);

    fprintf(stderr, "huffman table %p (log_table_size=%d, %d entries, entryies=%p):\n", table, table->log_table_size, table_size, table->entries);
    for (i = 0; i < table_size; i++) {
        fprintf(stderr, "%6d: PREFLEN=%d, RANGELEN=%d, ", i, table->entries[i].PREFLEN, table->entries[i].RANGELEN);
        if (table->entries[i].flags & JBIG2_HUFFMAN_FLAGS_ISEXT) {
            fprintf(stderr, "ext=%p", table->entries[i].u.ext_table);
        } else {
            fprintf(stderr, "RANGELOW=%d", table->entries[i].u.RANGELOW);
        }
        if (table->entries[i].flags) {
            int need_comma = 0;

            fprintf(stderr, ", flags=0x%x(", table->entries[i].flags);
            if (table->entries[i].flags & JBIG2_HUFFMAN_FLAGS_ISOOB) {
                fprintf(stderr, "OOB");
                need_comma = 1;
            }
            if (table->entries[i].flags & JBIG2_HUFFMAN_FLAGS_ISLOW) {
                if (need_comma)
                    fprintf(stderr, ",");
                fprintf(stderr, "LOW");
                need_comma = 1;
            }
            if (table->entries[i].flags & JBIG2_HUFFMAN_FLAGS_ISEXT) {
                if (need_comma)
                    fprintf(stderr, ",");
                fprintf(stderr, "EXT");
            }
            fprintf(stderr, ")");
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
}
