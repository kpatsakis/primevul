  virtual void DisconnectFromWirelessNetwork(const WirelessNetwork* network) {
    DCHECK(network);
    if (!EnsureCrosLoaded() || !network)
      return;
    if (DisconnectFromNetwork(network->service_path().c_str())) {
      if (network->type() == TYPE_WIFI) {
        WifiNetwork* wifi = GetWirelessNetworkByPath(
            wifi_networks_, network->service_path());
        if (wifi) {
          wifi->set_connected(false);
          wifi_ = NULL;
        }
      } else if (network->type() == TYPE_CELLULAR) {
        CellularNetwork* cellular = GetWirelessNetworkByPath(
            cellular_networks_, network->service_path());
        if (cellular) {
          cellular->set_connected(false);
          cellular_ = NULL;
        }
      }
      NotifyNetworkManagerChanged();
    }
  }
