static bool CheckAac(const uint8* buffer, int buffer_size) {
  RCHECK(buffer_size > 6);

  int offset = 0;
  while (offset + 6 < buffer_size) {
    BitReader reader(buffer + offset, 6);

    RCHECK(ReadBits(&reader, 12) == 0xfff);

    reader.SkipBits(1);

    RCHECK(ReadBits(&reader, 2) == 0);

    reader.SkipBits(1 + 2);

    RCHECK(ReadBits(&reader, 4) != 15);  // Forbidden.

    reader.SkipBits(1 + 3 + 1 + 1 + 1 + 1);

    int size = ReadBits(&reader, 13);
    RCHECK(size > 0);
    offset += size;
  }
  return true;
}
