int CountFilesCreatedAfter(const FilePath& path,
                           const base::Time& comparison_time) {
  base::ThreadRestrictions::AssertIOAllowed();
  int file_count = 0;

  DIR* dir = opendir(path.value().c_str());
  if (dir) {
#if !defined(OS_LINUX) && !defined(OS_MACOSX) && !defined(OS_BSD) && \
    !defined(OS_SOLARIS) && !defined(OS_ANDROID)
  #error Port warning: depending on the definition of struct dirent, \
         additional space for pathname may be needed
#endif
    struct dirent ent_buf;
    struct dirent* ent;
    while (readdir_r(dir, &ent_buf, &ent) == 0 && ent) {
      if ((strcmp(ent->d_name, ".") == 0) ||
          (strcmp(ent->d_name, "..") == 0))
        continue;

      stat_wrapper_t st;
      int test = CallStat(path.Append(ent->d_name).value().c_str(), &st);
      if (test != 0) {
        DPLOG(ERROR) << "stat64 failed";
        continue;
      }
      if (static_cast<time_t>(st.st_ctime) >= comparison_time.ToTimeT())
        ++file_count;
    }
    closedir(dir);
  }
  return file_count;
}
