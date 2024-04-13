  void SetUsageStat(DWORD value, bool medium) {
    ASSERT_TRUE(!medium || system_level_);
    ASSERT_EQ(ERROR_SUCCESS,
              base::win::RegKey(root_key_, GetUsageStatsKeyPath(medium).c_str(),
                                KEY_SET_VALUE | KEY_WOW64_32KEY)
                  .WriteValue(L"usagestats", value));
  }
