static BROTLI_NOINLINE BrotliResult SafeProcessCommands(BrotliState* s) {
  return ProcessCommandsInternal(1, s);
}
