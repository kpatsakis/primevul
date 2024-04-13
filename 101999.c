bool BluetoothSocketListenUsingL2capFunction::CreateParams() {
  params_ = bluetooth_socket::ListenUsingL2cap::Params::Create(*args_);
  return params_ != nullptr;
}
