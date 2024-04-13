void ShutdownSDK() {
  FPDF_DestroyLibrary();
  TearDownV8();
}
