int DCTStream::readAmp(int size) {
  int amp, bit;
  int bits;

  amp = 0;
  for (bits = 0; bits < size; ++bits) {
    if ((bit = readBit()) == EOF)
      return 9999;
    amp = (amp << 1) + bit;
  }
  if (amp < (1 << (size - 1)))
    amp -= (1 << size) - 1;
  return amp;
}
