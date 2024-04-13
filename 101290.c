void Document::setXMLStandalone(bool standalone,
                                ExceptionState& exception_state) {
  xml_standalone_ = standalone ? kStandalone : kNotStandalone;
}
