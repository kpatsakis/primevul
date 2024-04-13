CachedFileStream::~CachedFileStream()
{
  close();
  cc->decRefCnt();
}
