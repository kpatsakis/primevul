static BROTLI_INLINE int DecodeLiteralBlockSwitchInternal(int safe,
    BrotliState* s) {
  uint8_t context_mode;
  uint32_t context_offset;
  if (!DecodeBlockTypeAndLength(safe, s, 0)) {
    return 0;
  }
  context_offset = s->block_type_rb[1] << kLiteralContextBits;
  s->context_map_slice = s->context_map + context_offset;
  s->literal_htree_index = s->context_map_slice[0];
  s->literal_htree = s->literal_hgroup.htrees[s->literal_htree_index];
  context_mode = s->context_modes[s->block_type_rb[1]];
  s->context_lookup1 = &kContextLookup[kContextLookupOffsets[context_mode]];
  s->context_lookup2 = &kContextLookup[kContextLookupOffsets[context_mode + 1]];
  return 1;
}
