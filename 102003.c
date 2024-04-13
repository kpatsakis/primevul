ExtensionFunction::ResponseAction BluetoothSocketListenFunction::Run() {
  DCHECK_CURRENTLY_ON(work_thread_id());
  device::BluetoothAdapterFactory::GetAdapter(
      base::Bind(&BluetoothSocketListenFunction::OnGetAdapter, this));
  return did_respond() ? AlreadyResponded() : RespondLater();
}
