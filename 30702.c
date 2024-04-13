GBool FlateStream::readDynamicCodes() {
  int numCodeLenCodes;
  int numLitCodes;
  int numDistCodes;
  int codeLenCodeLengths[flateMaxCodeLenCodes];
  FlateHuffmanTab codeLenCodeTab;
  int len, repeat, code;
  int i;

  codeLenCodeTab.codes = NULL;

  if ((numLitCodes = getCodeWord(5)) == EOF) {
    goto err;
  }
  numLitCodes += 257;
  if ((numDistCodes = getCodeWord(5)) == EOF) {
    goto err;
  }
  numDistCodes += 1;
  if ((numCodeLenCodes = getCodeWord(4)) == EOF) {
    goto err;
  }
  numCodeLenCodes += 4;
  if (numLitCodes > flateMaxLitCodes ||
      numDistCodes > flateMaxDistCodes ||
      numCodeLenCodes > flateMaxCodeLenCodes) {
    goto err;
  }

  for (i = 0; i < flateMaxCodeLenCodes; ++i) {
    codeLenCodeLengths[i] = 0;
  }
  for (i = 0; i < numCodeLenCodes; ++i) {
    if ((codeLenCodeLengths[codeLenCodeMap[i]] = getCodeWord(3)) == -1) {
      goto err;
    }
  }
  compHuffmanCodes(codeLenCodeLengths, flateMaxCodeLenCodes, &codeLenCodeTab);

  len = 0;
  repeat = 0;
  i = 0;
  while (i < numLitCodes + numDistCodes) {
    if ((code = getHuffmanCodeWord(&codeLenCodeTab)) == EOF) {
      goto err;
    }
    if (code == 16) {
      if ((repeat = getCodeWord(2)) == EOF) {
	goto err;
      }
      repeat += 3;
      if (i + repeat > numLitCodes + numDistCodes) {
	goto err;
      }
      for (; repeat > 0; --repeat) {
	codeLengths[i++] = len;
      }
    } else if (code == 17) {
      if ((repeat = getCodeWord(3)) == EOF) {
	goto err;
      }
      repeat += 3;
      if (i + repeat > numLitCodes + numDistCodes) {
	goto err;
      }
      len = 0;
      for (; repeat > 0; --repeat) {
	codeLengths[i++] = 0;
      }
    } else if (code == 18) {
      if ((repeat = getCodeWord(7)) == EOF) {
	goto err;
      }
      repeat += 11;
      if (i + repeat > numLitCodes + numDistCodes) {
	goto err;
      }
      len = 0;
      for (; repeat > 0; --repeat) {
	codeLengths[i++] = 0;
      }
    } else {
      codeLengths[i++] = len = code;
    }
  }
  compHuffmanCodes(codeLengths, numLitCodes, &litCodeTab);
  compHuffmanCodes(codeLengths + numLitCodes, numDistCodes, &distCodeTab);

  gfree(codeLenCodeTab.codes);
  return gTrue;

err:
  error(errSyntaxError, getPos(), "Bad dynamic code table in flate stream");
  gfree(codeLenCodeTab.codes);
  return gFalse;
}
