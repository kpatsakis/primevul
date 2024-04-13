  gfx::Rect GetRectFromString(const std::string& str) {
    std::vector<std::string> tokens = base::SplitString(
        str, ",", base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY);
    EXPECT_EQ(4U, tokens.size());
    double x = 0.0, y = 0.0, width = 0.0, height = 0.0;
    EXPECT_TRUE(base::StringToDouble(tokens[0], &x));
    EXPECT_TRUE(base::StringToDouble(tokens[1], &y));
    EXPECT_TRUE(base::StringToDouble(tokens[2], &width));
    EXPECT_TRUE(base::StringToDouble(tokens[3], &height));
    return {static_cast<int>(x), static_cast<int>(y), static_cast<int>(width),
            static_cast<int>(height)};
  }
