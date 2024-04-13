static int dexOptMkdir(const char*  path, int mode)
{
#ifdef _WIN32
 return mkdir(path);
#else
 return mkdir(path, mode);
#endif
}
