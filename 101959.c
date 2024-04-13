quic::QuicTransportVersionVector ParseQuicVersions(
    const std::string& quic_versions) {
  quic::QuicTransportVersionVector supported_versions;
  quic::QuicTransportVersionVector all_supported_versions =
      quic::AllSupportedTransportVersions();

  for (const base::StringPiece& version : base::SplitStringPiece(
           quic_versions, ",", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL)) {
    auto it = all_supported_versions.begin();
    while (it != all_supported_versions.end()) {
      if (quic::QuicVersionToString(*it) == version) {
        supported_versions.push_back(*it);
        all_supported_versions.erase(it);
        break;
      }
      it++;
    }
  }
  return supported_versions;
}
