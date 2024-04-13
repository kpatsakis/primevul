void BluetoothSocketSendFunction::OnSuccess(int bytes_sent) {
  DCHECK_CURRENTLY_ON(work_thread_id());
  Respond(ArgumentList(bluetooth_socket::Send::Results::Create(bytes_sent)));
}
