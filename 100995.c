static bool CheckMp3(const uint8* buffer, int buffer_size, bool seenHeader) {
  RCHECK(buffer_size >= 10);  // Must be enough to read the initial header.

  int framesize;
  int numSeen = 0;
  int offset = 0;
  if (seenHeader) {
    offset = GetMp3HeaderSize(buffer, buffer_size);
  } else {
    while (offset < buffer_size && buffer[offset] == 0)
      ++offset;
  }

  while (offset + 3 < buffer_size) {
    RCHECK(ValidMpegAudioFrameHeader(
        buffer + offset, buffer_size - offset, &framesize));

    if (++numSeen > 10)
      return true;
    offset += framesize;
  }
  return numSeen > 2;
}
