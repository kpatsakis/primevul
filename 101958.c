quic::QuicTagVector GetQuicClientConnectionOptions(
    const VariationParameters& quic_trial_params) {
  auto it = quic_trial_params.find("client_connection_options");
  if (it == quic_trial_params.end()) {
    return quic::QuicTagVector();
  }

  return net::ParseQuicConnectionOptions(it->second);
}
