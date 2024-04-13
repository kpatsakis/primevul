void RunElisionTest(const std::vector<Testcase>& testcases) {
  const gfx::FontList font_list;
  for (const auto& testcase : testcases) {
    SCOPED_TRACE("Eliding " + testcase.input);
    const GURL url(testcase.input);
    const float available_width =
        gfx::GetStringWidthF(base::UTF8ToUTF16(testcase.output), font_list);
    EXPECT_EQ(base::UTF8ToUTF16(testcase.output),
              url_formatter::ElideUrl(url, font_list, available_width));
  }
}
