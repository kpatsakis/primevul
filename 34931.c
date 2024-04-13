test_get_word(Jbig2WordStream *self, int offset, uint32_t *word)
{
    uint32_t val = 0;
    test_stream_t *st = (test_stream_t *) self;

    if (st != NULL) {
        if (st->h != NULL) {
            if (offset >= st->h->input_len)
                return -1;
            if (offset < st->h->input_len) {
                val |= (st->h->input[offset] << 24);
            }
            if (offset + 1 < st->h->input_len) {
                val |= (st->h->input[offset + 1] << 16);
            }
            if (offset + 2 < st->h->input_len) {
                val |= (st->h->input[offset + 2] << 8);
            }
            if (offset + 3 < st->h->input_len) {
                val |= st->h->input[offset + 3];
            }
        }
    }
    *word = val;
    return 0;
}
