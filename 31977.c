char *FoFiType1::getName() {
  if (!parsed) {
    parse();
  }
  return name;
}
