int HttpStreamParser::EncodeChunk(const base::StringPiece& payload,
                                  char* output,
                                  size_t output_size) {
  if (output_size < payload.size() + kChunkHeaderFooterSize)
    return ERR_INVALID_ARGUMENT;

  char* cursor = output;
  const int num_chars = base::snprintf(output, output_size,
                                       "%X\r\n",
                                       static_cast<int>(payload.size()));
  cursor += num_chars;
  if (payload.size() > 0) {
    memcpy(cursor, payload.data(), payload.size());
    cursor += payload.size();
  }
  memcpy(cursor, "\r\n", 2);
  cursor += 2;

  return cursor - output;
}
