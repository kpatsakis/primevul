FoFiType1 *FoFiType1::load(char *fileName) {
  char *fileA;
  int lenA;

  if (!(fileA = FoFiBase::readFile(fileName, &lenA))) {
    return NULL;
  }
  return new FoFiType1(fileA, lenA, gTrue);
}
