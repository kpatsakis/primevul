bool HasNonEmptyLocationHeader(const FetchHeaderList* headers) {
  String value;
  if (!headers->Get(HTTPNames::Location, value))
    return false;
  return !value.IsEmpty();
}
