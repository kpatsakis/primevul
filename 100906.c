static int CallStat(const char *path, stat_wrapper_t *sb) {
  base::ThreadRestrictions::AssertIOAllowed();
  return stat(path, sb);
}
