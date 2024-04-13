set_local_file (const char **file, const char *default_file)
{
  if (opt.output_document)
    {
      if (output_stream_regular)
        *file = opt.output_document;
    }
  else
    *file = default_file;
}
