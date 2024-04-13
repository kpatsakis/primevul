jbig2_dump_huffman_binary(Jbig2HuffmanState *hs)
{
    const uint32_t word = hs->this_word;
    int i;

    fprintf(stderr, "huffman binary ");
    for (i = 31; i >= 0; i--)
        fprintf(stderr, ((word >> i) & 1) ? "1" : "0");
    fprintf(stderr, "\n");
}
