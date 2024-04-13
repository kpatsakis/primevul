CCITTFaxStream::CCITTFaxStream(Stream *strA, int encodingA, GBool endOfLineA,
			       GBool byteAlignA, int columnsA, int rowsA,
			       GBool endOfBlockA, GBool blackA):
    FilterStream(strA) {
  encoding = encodingA;
  endOfLine = endOfLineA;
  byteAlign = byteAlignA;
  columns = columnsA;
  if (columns < 1) {
    columns = 1;
  } else if (columns > INT_MAX - 2) {
    columns = INT_MAX - 2;
  }
  rows = rowsA;
  endOfBlock = endOfBlockA;
  black = blackA;
  codingLine = (int *)gmallocn_checkoverflow(columns + 1, sizeof(int));
  refLine = (int *)gmallocn_checkoverflow(columns + 2, sizeof(int));

  if (codingLine != NULL && refLine != NULL) {
    eof = gFalse;
    codingLine[0] = columns;
  } else {
    eof = gTrue;
  }
  row = 0;
  nextLine2D = encoding < 0;
  inputBits = 0;
  a0i = 0;
  outputBits = 0;

  buf = EOF;
}
