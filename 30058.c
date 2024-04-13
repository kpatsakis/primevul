report_revision (bool found_revision)
{
  char const *rev = quotearg (revision);

  if (found_revision)
    {
      if (verbosity == VERBOSE)
	say ("Good.  This file appears to be the %s version.\n", rev);
    }
  else if (force)
    {
      if (verbosity != SILENT)
	say ("Warning: this file doesn't appear to be the %s version -- patching anyway.\n",
	     rev);
    }
  else if (batch)
    fatal ("This file doesn't appear to be the %s version -- aborting.",
	   rev);
  else
    {
      ask ("This file doesn't appear to be the %s version -- patch anyway? [n] ",
	   rev);
      if (*buf != 'y')
	fatal ("aborted");
    }
}
