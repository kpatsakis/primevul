int Stream::getRawChar() {
  error(errInternal, -1, "Internal: called getRawChar() on non-predictor stream");
  return EOF;
}
