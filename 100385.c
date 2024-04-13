void NavigationController::InsertEntriesFrom(
    const NavigationController& source,
    int max_index) {
  DCHECK_LE(max_index, source.entry_count());
  size_t insert_index = 0;
  for (int i = 0; i < max_index; i++) {
    if (source.entries_[i].get()->page_type() != INTERSTITIAL_PAGE) {
      entries_.insert(entries_.begin() + insert_index++,
                      linked_ptr<NavigationEntry>(
                          new NavigationEntry(*source.entries_[i])));
    }
  }
}
