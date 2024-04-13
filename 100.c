WirelessNetwork::WirelessNetwork(const ServiceInfo* service)
    : Network(service) {
  name_ = SafeString(service->name);
  strength_ = service->strength;
  auto_connect_ = service->auto_connect;
  favorite_ = service->favorite;
}
