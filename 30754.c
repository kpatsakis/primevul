FlateStream::~FlateStream() {
  if (litCodeTab.codes != fixedLitCodeTab.codes) {
    gfree(litCodeTab.codes);
  }
  if (distCodeTab.codes != fixedDistCodeTab.codes) {
    gfree(distCodeTab.codes);
  }
  if (pred) {
    delete pred;
  }
  delete str;
}
