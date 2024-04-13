bool HasFileBeenModifiedSince(const FileEnumerator::FindInfo& find_info,
                              const base::Time& cutoff_time) {
  return static_cast<time_t>(find_info.stat.st_mtime) >= cutoff_time.ToTimeT();
}
