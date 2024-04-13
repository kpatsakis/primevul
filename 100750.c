int CopyBaseDriveSpecIfNecessary(const char* base_url,
                                 int base_path_begin,
                                 int base_path_end,
                                 const CHAR* relative_url,
                                 int path_start,
                                 int relative_url_len,
                                 CanonOutput* output) {
  if (base_path_begin >= base_path_end)
    return base_path_begin;  // No path.

  if (url_parse::DoesBeginWindowsDriveSpec(relative_url,
                                           path_start, relative_url_len)) {
    return base_path_begin;  // Relative URL path is "C:/foo"
  }

  if (DoesBeginSlashWindowsDriveSpec(base_url,
                                     base_path_begin,
                                     base_path_end)) {
    output->push_back('/');
    output->push_back(base_url[base_path_begin + 1]);
    output->push_back(base_url[base_path_begin + 2]);
    return base_path_begin + 3;
  }

  return base_path_begin;
}
