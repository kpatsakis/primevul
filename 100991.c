static bool CheckAc3(const uint8* buffer, int buffer_size) {

  RCHECK(buffer_size > 6);

  int offset = 0;
  while (offset + 6 < buffer_size) {
    BitReader reader(buffer + offset, 6);

    RCHECK(ReadBits(&reader, 16) == kAc3SyncWord);

    reader.SkipBits(16);

    int sample_rate_code = ReadBits(&reader, 2);
    RCHECK(sample_rate_code != 3);  // Reserved.

    int frame_size_code = ReadBits(&reader, 6);
    RCHECK(frame_size_code < 38);  // Undefined.

    RCHECK(ReadBits(&reader, 5) < 10);  // Normally 8 or 6, 16 used by EAC3.

    offset += kAc3FrameSizeTable[frame_size_code][sample_rate_code];
  }
  return true;
}
