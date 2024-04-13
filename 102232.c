bool Camera3Device::RequestThread::isStreamPending(
        sp<Camera3StreamInterface>& stream) {
 Mutex::Autolock l(mRequestLock);

 for (const auto& nextRequest : mNextRequests) {
 if (!nextRequest.submitted) {
 for (const auto& s : nextRequest.captureRequest->mOutputStreams) {
 if (stream == s) return true;
 }
 if (stream == nextRequest.captureRequest->mInputStream) return true;
 }
 }

 for (const auto& request : mRequestQueue) {
 for (const auto& s : request->mOutputStreams) {
 if (stream == s) return true;
 }
 if (stream == request->mInputStream) return true;
 }

 for (const auto& request : mRepeatingRequests) {
 for (const auto& s : request->mOutputStreams) {
 if (stream == s) return true;
 }
 if (stream == request->mInputStream) return true;
 }

 return false;
}
