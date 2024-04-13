void MediaPlayerService::Client::disconnectNativeWindow() {
 if (mConnectedWindow != NULL) {
 status_t err = native_window_api_disconnect(mConnectedWindow.get(),
                NATIVE_WINDOW_API_MEDIA);

 if (err != OK) {
            ALOGW("native_window_api_disconnect returned an error: %s (%d)",
                    strerror(-err), err);
 }
 }
    mConnectedWindow.clear();
}
