void FoFiType1::writeEncoded(char **newEncoding,
			     FoFiOutputFunc outputFunc, void *outputStream) {
  char buf[512];
  char *line, *line2, *p;
  int i;

  for (line = (char *)file;
       line && strncmp(line, "/Encoding", 9);
       line = getNextLine(line)) ;
  if (!line) {
    (*outputFunc)(outputStream, (char *)file, len);
    return;
  }
  (*outputFunc)(outputStream, (char *)file, line - (char *)file);

  (*outputFunc)(outputStream, "/Encoding 256 array\n", 20);
  (*outputFunc)(outputStream,
		"0 1 255 {1 index exch /.notdef put} for\n", 40);
  for (i = 0; i < 256; ++i) {
    if (newEncoding[i]) {
      sprintf(buf, "dup %d /%s put\n", i, newEncoding[i]);
      (*outputFunc)(outputStream, buf, strlen(buf));
    }
  }
  (*outputFunc)(outputStream, "readonly def\n", 13);
  
  if (!strncmp(line, "/Encoding StandardEncoding def", 30)) {
    line = getNextLine(line);
  } else {
    p = line + 10;
    line = NULL;
    for (; p < (char *)file + len; ++p) {
      if ((*p == ' ' || *p == '\t' || *p == '\x0a' ||
	   *p == '\x0d' || *p == '\x0c' || *p == '\0') &&
	  p + 4 <= (char *)file + len &&
	  !strncmp(p + 1, "def", 3)) {
	line = p + 4;
	break;
      }
    }
  }

  if (line) {
    for (line2 = line, i = 0;
	 i < 20 && line2 && strncmp(line2, "/Encoding", 9);
	 line2 = getNextLine(line2), ++i) ;
    if (i < 20 && line2) {
      (*outputFunc)(outputStream, line, line2 - line);
      if (!strncmp(line2, "/Encoding StandardEncoding def", 30)) {
	line = getNextLine(line2);
      } else {
	p = line2 + 10;
	line = NULL;
	for (; p < (char *)file + len; ++p) {
	  if ((*p == ' ' || *p == '\t' || *p == '\x0a' ||
	       *p == '\x0d' || *p == '\x0c' || *p == '\0') &&
	      p + 4 <= (char *)file + len &&
	      !strncmp(p + 1, "def", 3)) {
	    line = p + 4;
	    break;
	  }
	}
      }
    }

    if (line) {
      (*outputFunc)(outputStream, line, ((char *)file + len) - line);
    }
  }
}
