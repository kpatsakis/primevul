jbig2_huffman_skip(Jbig2HuffmanState *hs)
{
    int bits = hs->offset_bits & 7;

    if (bits) {
        bits = 8 - bits;
        hs->offset_bits += bits;
        hs->this_word = (hs->this_word << bits) | (hs->next_word >> (32 - hs->offset_bits));
    }

    if (hs->offset_bits >= 32) {
        hs->this_word = hs->next_word;
        hs->offset += 4;
        hs->next_word = huff_get_next_word(hs, hs->offset + 4);
        hs->offset_bits -= 32;
        if (hs->offset_bits) {
            hs->this_word = (hs->this_word << hs->offset_bits) | (hs->next_word >> (32 - hs->offset_bits));
        }
    }
}
