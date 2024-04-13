void DCTStream::restart() {
  int i;

  inputBits = 0;
  restartCtr = restartInterval;
  for (i = 0; i < numComps; ++i) {
    compInfo[i].prevDC = 0;
  }
  eobRun = 0;
}
