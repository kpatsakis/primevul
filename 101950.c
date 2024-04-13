bool ProcessNeedsProfileDir(const std::string& process_type) {
  return ProcessNeedsProfileDir(GetProcessType(UTF8ToUTF16(process_type)));
}
