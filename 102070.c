std::string TestURLLoader::GetReachableCrossOriginURL(
    const std::string& file_name) {
  std::string url = GetReachableAbsoluteURL(file_name);
  std::string host("127.0.0.1");
  size_t index = url.find(host);
  ASSERT_NE(index, std::string::npos);
  url.replace(index, host.length(), "localhost");
  return url;
}
