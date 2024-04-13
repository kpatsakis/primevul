status_t Camera3Device::captureList(const List<const CameraMetadata> &requests,
 int64_t *lastFrameNumber) {
    ATRACE_CALL();

 return submitRequestsHelper(requests, /*repeating*/false, lastFrameNumber);
}
