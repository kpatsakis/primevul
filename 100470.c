AudioOutputController::AudioOutputController(EventHandler* handler,
                                             uint32 capacity,
                                             SyncReader* sync_reader)
    : handler_(handler),
      stream_(NULL),
      volume_(1.0),
      state_(kEmpty),
      buffer_(0, capacity),
      pending_request_(false),
      sync_reader_(sync_reader),
      message_loop_(NULL) {
}
