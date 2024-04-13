void NotifyTimezoneChangeOnThisThread() {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  if (!isolate)
    return;
  v8::Date::DateTimeConfigurationChangeNotification(isolate);
}
