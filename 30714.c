void FileStream::reset() {
#if HAVE_FSEEKO
  savePos = (Guint)ftello(f);
  fseeko(f, start, SEEK_SET);
#elif HAVE_FSEEK64
  savePos = (Guint)ftell64(f);
  fseek64(f, start, SEEK_SET);
#else
  savePos = (Guint)ftell(f);
  fseek(f, start, SEEK_SET);
#endif
  saved = gTrue;
  bufPtr = bufEnd = buf;
  bufPos = start;
}
