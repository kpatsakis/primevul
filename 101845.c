void GamepadProvider::DoRemoveSourceGamepadDataFetcher(GamepadSource source) {
  DCHECK(polling_thread_->task_runner()->BelongsToCurrentThread());

  for (GamepadFetcherVector::iterator it = data_fetchers_.begin();
       it != data_fetchers_.end();) {
    if ((*it)->source() == source) {
      it = data_fetchers_.erase(it);
    } else {
      ++it;
    }
  }
}
