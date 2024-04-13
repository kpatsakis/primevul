void BookmarksUpdateFunction::GetQuotaLimitHeuristics(
    QuotaLimitHeuristics* heuristics) const {
  BookmarksQuotaLimitFactory::Build<BookmarksUpdateFunction>(heuristics);
};
