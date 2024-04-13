 jbig2_huffman_offset(Jbig2HuffmanState *hs)
 {
     return hs->offset + (hs->offset_bits >> 3);
}
