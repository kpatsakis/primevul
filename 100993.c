static bool CheckGsm(const uint8* buffer, int buffer_size) {
  RCHECK(buffer_size >= 1024);  // Need enough data to do a decent check.

  int offset = 0;
  while (offset < buffer_size) {
    RCHECK((buffer[offset] & 0xf0) == 0xd0);
    offset += 33;
  }
  return true;
}
