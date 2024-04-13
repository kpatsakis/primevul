void FileStream::close() {
  if (saved) {
#if HAVE_FSEEKO
    fseeko(f, savePos, SEEK_SET);
#elif HAVE_FSEEK64
    fseek64(f, savePos, SEEK_SET);
#else
    fseek(f, savePos, SEEK_SET);
#endif
    saved = gFalse;
  }
}
