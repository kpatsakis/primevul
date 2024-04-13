void CCITTFaxStream::ccittReset(GBool unfiltered) {
  if (unfiltered)
    str->unfilteredReset();
  else
    str->reset();

  row = 0;
  nextLine2D = encoding < 0;
  inputBits = 0;
  a0i = 0;
  outputBits = 0;
  buf = EOF;
}
