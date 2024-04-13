void FlateStream::flateReset(GBool unfiltered) {
  if (unfiltered)
    str->unfilteredReset();
  else
    str->reset();

  index = 0;
  remain = 0;
  codeBuf = 0;
  codeSize = 0;
  compressedBlock = gFalse;
  endOfBlock = gTrue;
  eof = gTrue;
}
