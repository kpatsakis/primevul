void BluetoothSocketAbstractConnectFunction::OnGetAdapter(
    scoped_refptr<device::BluetoothAdapter> adapter) {
  DCHECK_CURRENTLY_ON(work_thread_id());
  BluetoothApiSocket* socket = GetSocket(params_->socket_id);
  if (!socket) {
    Respond(Error(kSocketNotFoundError));
    return;
  }

  device::BluetoothDevice* device = adapter->GetDevice(params_->address);
  if (!device) {
    Respond(Error(kDeviceNotFoundError));
    return;
  }

  device::BluetoothUUID uuid(params_->uuid);
  if (!uuid.IsValid()) {
    Respond(Error(kInvalidUuidError));
    return;
  }

  BluetoothPermissionRequest param(params_->uuid);
  if (!BluetoothManifestData::CheckRequest(extension(), param)) {
    Respond(Error(kPermissionDeniedError));
    return;
  }

  ConnectToService(device, uuid);
}
