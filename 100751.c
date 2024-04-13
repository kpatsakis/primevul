bool DoesBeginSlashWindowsDriveSpec(const CHAR* spec, int start_offset,
                                    int spec_len) {
  if (start_offset >= spec_len)
    return false;
  return url_parse::IsURLSlash(spec[start_offset]) &&
      url_parse::DoesBeginWindowsDriveSpec(spec, start_offset + 1, spec_len);
}
