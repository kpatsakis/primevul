input_file_url (const char *input_file)
{
  static bool first = true;

  if (input_file
      && url_has_scheme (input_file)
      && first)
    {
      first = false;
      return true;
    }
  else
    return false;
}
