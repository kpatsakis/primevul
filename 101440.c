std::string XmlConfigParser::XMLCharToString(const xmlChar* value) {
  return std::string(reinterpret_cast<const char*>(value));
}
