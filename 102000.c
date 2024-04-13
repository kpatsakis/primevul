BluetoothSocketEventDispatcher* GetSocketEventDispatcher(
    content::BrowserContext* browser_context) {
  BluetoothSocketEventDispatcher* socket_event_dispatcher =
      BluetoothSocketEventDispatcher::Get(browser_context);
  DCHECK(socket_event_dispatcher)
      << "There is no socket event dispatcher. "
         "If this assertion is failing during a test, then it is likely that "
         "TestExtensionSystem is failing to provide an instance of "
         "BluetoothSocketEventDispatcher.";
  return socket_event_dispatcher;
}
