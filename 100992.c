static bool CheckBink(const uint8* buffer, int buffer_size) {
  RCHECK(buffer_size >= 44);

  RCHECK(Read32LE(buffer + 8) > 0);

  int width = Read32LE(buffer + 20);
  RCHECK(width > 0 && width <= 32767);

  int height = Read32LE(buffer + 24);
  RCHECK(height > 0 && height <= 32767);

  RCHECK(Read32LE(buffer + 28) > 0);

  RCHECK(Read32LE(buffer + 32) > 0);

  return (Read32LE(buffer + 40) <= 256);
}
