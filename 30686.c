Stream *Stream::makeFilter(char *name, Stream *str, Object *params) {
  int pred;			// parameters
  int colors;
  int bits;
  int early;
  int encoding;
  GBool endOfLine, byteAlign, endOfBlock, black;
  int columns, rows;
  int colorXform;
  Object globals, obj;

  if (!strcmp(name, "ASCIIHexDecode") || !strcmp(name, "AHx")) {
    str = new ASCIIHexStream(str);
  } else if (!strcmp(name, "ASCII85Decode") || !strcmp(name, "A85")) {
    str = new ASCII85Stream(str);
  } else if (!strcmp(name, "LZWDecode") || !strcmp(name, "LZW")) {
    pred = 1;
    columns = 1;
    colors = 1;
    bits = 8;
    early = 1;
    if (params->isDict()) {
      params->dictLookup("Predictor", &obj);
      if (obj.isInt())
	pred = obj.getInt();
      obj.free();
      params->dictLookup("Columns", &obj);
      if (obj.isInt())
	columns = obj.getInt();
      obj.free();
      params->dictLookup("Colors", &obj);
      if (obj.isInt())
	colors = obj.getInt();
      obj.free();
      params->dictLookup("BitsPerComponent", &obj);
      if (obj.isInt())
	bits = obj.getInt();
      obj.free();
      params->dictLookup("EarlyChange", &obj);
      if (obj.isInt())
	early = obj.getInt();
      obj.free();
    }
    str = new LZWStream(str, pred, columns, colors, bits, early);
  } else if (!strcmp(name, "RunLengthDecode") || !strcmp(name, "RL")) {
    str = new RunLengthStream(str);
  } else if (!strcmp(name, "CCITTFaxDecode") || !strcmp(name, "CCF")) {
    encoding = 0;
    endOfLine = gFalse;
    byteAlign = gFalse;
    columns = 1728;
    rows = 0;
    endOfBlock = gTrue;
    black = gFalse;
    if (params->isDict()) {
      params->dictLookup("K", &obj);
      if (obj.isInt()) {
	encoding = obj.getInt();
      }
      obj.free();
      params->dictLookup("EndOfLine", &obj);
      if (obj.isBool()) {
	endOfLine = obj.getBool();
      }
      obj.free();
      params->dictLookup("EncodedByteAlign", &obj);
      if (obj.isBool()) {
	byteAlign = obj.getBool();
      }
      obj.free();
      params->dictLookup("Columns", &obj);
      if (obj.isInt()) {
	columns = obj.getInt();
      }
      obj.free();
      params->dictLookup("Rows", &obj);
      if (obj.isInt()) {
	rows = obj.getInt();
      }
      obj.free();
      params->dictLookup("EndOfBlock", &obj);
      if (obj.isBool()) {
	endOfBlock = obj.getBool();
      }
      obj.free();
      params->dictLookup("BlackIs1", &obj);
      if (obj.isBool()) {
	black = obj.getBool();
      }
      obj.free();
    }
    str = new CCITTFaxStream(str, encoding, endOfLine, byteAlign,
			     columns, rows, endOfBlock, black);
  } else if (!strcmp(name, "DCTDecode") || !strcmp(name, "DCT")) {
    colorXform = -1;
    if (params->isDict()) {
      if (params->dictLookup("ColorTransform", &obj)->isInt()) {
	colorXform = obj.getInt();
      }
      obj.free();
    }
    str = new DCTStream(str, colorXform);
  } else if (!strcmp(name, "FlateDecode") || !strcmp(name, "Fl")) {
    pred = 1;
    columns = 1;
    colors = 1;
    bits = 8;
    if (params->isDict()) {
      params->dictLookup("Predictor", &obj);
      if (obj.isInt())
	pred = obj.getInt();
      obj.free();
      params->dictLookup("Columns", &obj);
      if (obj.isInt())
	columns = obj.getInt();
      obj.free();
      params->dictLookup("Colors", &obj);
      if (obj.isInt())
	colors = obj.getInt();
      obj.free();
      params->dictLookup("BitsPerComponent", &obj);
      if (obj.isInt())
	bits = obj.getInt();
      obj.free();
    }
    str = new FlateStream(str, pred, columns, colors, bits);
  } else if (!strcmp(name, "JBIG2Decode")) {
    if (params->isDict()) {
      params->dictLookup("JBIG2Globals", &globals);
    }
    str = new JBIG2Stream(str, &globals);
    globals.free();
  } else if (!strcmp(name, "JPXDecode")) {
    str = new JPXStream(str);
  } else {
    error(errSyntaxError, getPos(), "Unknown filter '{0:s}'", name);
    str = new EOFStream(str);
  }
  return str;
}
