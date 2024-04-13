int FlateStream::getHuffmanCodeWord(FlateHuffmanTab *tab) {
  FlateCode *code;
  int c;

  while (codeSize < tab->maxLen) {
    if ((c = str->getChar()) == EOF) {
      break;
    }
    codeBuf |= (c & 0xff) << codeSize;
    codeSize += 8;
  }
  code = &tab->codes[codeBuf & ((1 << tab->maxLen) - 1)];
  if (codeSize == 0 || codeSize < code->len || code->len == 0) {
    return EOF;
  }
  codeBuf >>= code->len;
  codeSize -= code->len;
  return (int)code->val;
}
