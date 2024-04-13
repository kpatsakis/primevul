void BrowserCommandController::RemoveCommandObserver(
    int id, CommandObserver* observer) {
  command_updater_.RemoveCommandObserver(id, observer);
}
