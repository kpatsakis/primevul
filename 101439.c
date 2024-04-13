void XmlConfigParser::Parse(const std::string& input_buffer,
                            std::string* output_buffer) {
  using logging::LOG_WARNING;

  DCHECK(output_buffer);
  xmlSAXHandler sax_handler = {};
  sax_handler.startElement = &XmlConfigParser::StartElementImpl;
  sax_handler.endElement = &XmlConfigParser::EndElementImpl;
  sax_handler.characters = &XmlConfigParser::CharactersImpl;
  XmlConfigParser parser;
  int error = xmlSAXUserParseMemory(&sax_handler,
                                    &parser,
                                    input_buffer.c_str(),
                                    input_buffer.size());
  if (error) {
    VLOG(LOG_WARNING) << "Error parsing brandcoded master prefs, err=" << error;
  } else {
    output_buffer->swap(parser.master_prefs_);
  }
}
