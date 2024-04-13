int FileOutStream::getPos ()
{
  return ftell(f);
}
