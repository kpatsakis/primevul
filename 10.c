  virtual void AddNetworkManagerObserver(NetworkManagerObserver* observer) {
    if (!network_manager_observers_.HasObserver(observer))
      network_manager_observers_.AddObserver(observer);
  }
