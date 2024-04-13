ImageProcessorClient::~ImageProcessorClient() {
  DCHECK_CALLED_ON_VALID_THREAD(test_main_thread_checker_);
  CHECK(image_processor_client_thread_.IsRunning());
  image_processor_client_thread_.task_runner()->DeleteSoon(
      FROM_HERE, image_processor_.release());
  image_processor_client_thread_.Stop();
}
