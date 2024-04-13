bool ImageProcessorClient::WaitForFrameProcessors() {
  bool success = true;
  for (auto& processor : frame_processors_)
    success &= processor->WaitUntilDone();

  return success;
}
