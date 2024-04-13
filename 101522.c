int PDFiumEngine::Form_GetPlatform(FPDF_FORMFILLINFO* param,
                                   void* platform,
                                   int length) {
  int platform_flag = -1;

#if defined(WIN32)
  platform_flag = 0;
#elif defined(__linux__)
  platform_flag = 1;
#else
  platform_flag = 2;
#endif

  std::string javascript = "alert(\"Platform:"
      + base::DoubleToString(platform_flag)
      + "\")";

  return platform_flag;
}
