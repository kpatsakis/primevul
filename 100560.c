SSLErrorInfo::SSLErrorInfo(const string16& title,
                           const string16& details,
                           const string16& short_description,
                           const std::vector<string16>& extra_info)
    : title_(title),
      details_(details),
      short_description_(short_description),
      extra_information_(extra_info) {
}
