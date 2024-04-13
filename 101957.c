void FileReaderLoader::UnadjustReportedMemoryUsageToV8() {
  if (!memory_usage_reported_to_v8_)
    return;
  v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(
      -memory_usage_reported_to_v8_);
  memory_usage_reported_to_v8_ = 0;
}
