void DCTStream::dctReset(GBool unfiltered) {
  if (unfiltered)
    str->unfilteredReset();
  else
    str->reset();

  progressive = interleaved = gFalse;
  width = height = 0;
  numComps = 0;
  numQuantTables = 0;
  numDCHuffTables = 0;
  numACHuffTables = 0;
  gotJFIFMarker = gFalse;
  gotAdobeMarker = gFalse;
  restartInterval = 0;
}
