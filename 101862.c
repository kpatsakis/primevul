uint64_t Core::ConnectToPeer(ConnectionParams connection_params,
                             const ports::PortRef& port) {
  RequestContext request_context;
  return GetNodeController()->ConnectToPeer(std::move(connection_params), port);
}
