inline unsigned char HexToInt(unsigned char c) {
  DCHECK(IsHexDigit(c));
  static unsigned char kOffset[4] = {0, 0x30u, 0x37u, 0x57u};
  return c - kOffset[(c >> 5) & 3];
}
