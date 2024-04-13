static BROTLI_INLINE int ReadCommandInternal(int safe,
    BrotliState* s, BrotliBitReader* br, int* insert_length) {
  uint32_t cmd_code;
  uint32_t insert_len_extra = 0;
  uint32_t copy_length;
  CmdLutElement v;
  BrotliBitReaderState memento;
  if (!safe) {
    cmd_code = ReadSymbol(s->htree_command, br);
  } else {
    BrotliBitReaderSaveState(br, &memento);
    if (!SafeReadSymbol(s->htree_command, br, &cmd_code)) {
      return 0;
    }
  }
  v = kCmdLut[cmd_code];
  s->distance_code = v.distance_code;
  s->distance_context = v.context;
  s->dist_htree_index = s->dist_context_map_slice[s->distance_context];
  *insert_length = v.insert_len_offset;
  if (!safe) {
    if (PREDICT_FALSE(v.insert_len_extra_bits != 0)) {
      insert_len_extra = BrotliReadBits(br, v.insert_len_extra_bits);
    }
    copy_length = BrotliReadBits(br, v.copy_len_extra_bits);
  } else {
    if (!SafeReadBits(br, v.insert_len_extra_bits, &insert_len_extra) ||
        !SafeReadBits(br, v.copy_len_extra_bits, &copy_length)) {
      BrotliBitReaderRestoreState(br, &memento);
      return 0;
    }
  }
  s->copy_length = (int)copy_length + v.copy_len_offset;
  --s->block_length[1];
  *insert_length += (int)insert_len_extra;
  return 1;
}
