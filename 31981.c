FoFiType1::~FoFiType1() {
  int i;

  if (name) {
    gfree(name);
  }
  if (encoding && encoding != fofiType1StandardEncoding) {
    for (i = 0; i < 256; ++i) {
      gfree(encoding[i]);
    }
    gfree(encoding);
  }
}
