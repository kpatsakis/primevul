void FileStream::setPos(Guint pos, int dir) {
  Guint size;

  if (dir >= 0) {
#if HAVE_FSEEKO
    fseeko(f, pos, SEEK_SET);
#elif HAVE_FSEEK64
    fseek64(f, pos, SEEK_SET);
#else
    fseek(f, pos, SEEK_SET);
#endif
    bufPos = pos;
  } else {
#if HAVE_FSEEKO
    fseeko(f, 0, SEEK_END);
    size = (Guint)ftello(f);
#elif HAVE_FSEEK64
    fseek64(f, 0, SEEK_END);
    size = (Guint)ftell64(f);
#else
    fseek(f, 0, SEEK_END);
    size = (Guint)ftell(f);
#endif
    if (pos > size)
      pos = (Guint)size;
#if HAVE_FSEEKO
    fseeko(f, -(int)pos, SEEK_END);
    bufPos = (Guint)ftello(f);
#elif HAVE_FSEEK64
    fseek64(f, -(int)pos, SEEK_END);
    bufPos = (Guint)ftell64(f);
#else
    fseek(f, -(int)pos, SEEK_END);
    bufPos = (Guint)ftell(f);
#endif
  }
  bufPtr = bufEnd = buf;
}
