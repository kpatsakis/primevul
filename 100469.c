SandboxedExtensionUnpacker::~SandboxedExtensionUnpacker() {
  base::FileUtilProxy::Delete(
      BrowserThread::GetMessageLoopProxyForThread(thread_identifier_),
      temp_dir_.Take(),
      true,
      NULL);
}
