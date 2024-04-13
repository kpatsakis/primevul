void FileOutStream::printf(const char *format, ...)
{
  va_list argptr;
  va_start (argptr, format);
  vfprintf(f, format, argptr);
  va_end (argptr);
}
