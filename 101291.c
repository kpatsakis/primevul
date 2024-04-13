void Document::setXMLVersion(const String& version,
                             ExceptionState& exception_state) {
  if (!XMLDocumentParser::SupportsXMLVersion(version)) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kNotSupportedError,
        "This document does not support the XML version '" + version + "'.");
    return;
  }

  xml_version_ = version;
}
