void Document::SetParsingState(ParsingState parsing_state) {
  parsing_state_ = parsing_state;

  if (Parsing() && !element_data_cache_)
    element_data_cache_ = ElementDataCache::Create();
}
