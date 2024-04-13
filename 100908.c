FILE* CreateAndOpenTemporaryShmemFile(FilePath* path, bool executable) {
  FilePath directory;
  if (!GetShmemTempDir(&directory, executable))
    return NULL;

  return CreateAndOpenTemporaryFileInDir(directory, path);
}
