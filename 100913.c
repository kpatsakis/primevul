bool PathExists(const FilePath& path) {
  base::ThreadRestrictions::AssertIOAllowed();
  return access(path.value().c_str(), F_OK) == 0;
}
