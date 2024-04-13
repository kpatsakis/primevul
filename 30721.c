void FlateStream::reset() {
  int cmf, flg;

  flateReset(gFalse);

  endOfBlock = eof = gTrue;
  cmf = str->getChar();
  flg = str->getChar();
  if (cmf == EOF || flg == EOF)
    return;
  if ((cmf & 0x0f) != 0x08) {
    error(errSyntaxError, getPos(), "Unknown compression method in flate stream");
    return;
  }
  if ((((cmf << 8) + flg) % 31) != 0) {
    error(errSyntaxError, getPos(), "Bad FCHECK in flate stream");
    return;
  }
  if (flg & 0x20) {
    error(errSyntaxError, getPos(), "FDICT bit set in flate stream");
    return;
  }

  eof = gFalse;
}
