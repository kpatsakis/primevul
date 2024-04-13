bool GetInode(const FilePath& path, ino_t* inode) {
  base::ThreadRestrictions::AssertIOAllowed();  // For call to stat().
  struct stat buffer;
  int result = stat(path.value().c_str(), &buffer);
  if (result < 0)
    return false;

  *inode = buffer.st_ino;
  return true;
}
