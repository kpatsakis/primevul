char **FoFiType1::getEncoding() {
  if (!parsed) {
    parse();
  }
  return encoding;
}
