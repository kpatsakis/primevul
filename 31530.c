void Smb4KGlobal::openShare(Smb4KShare *share, OpenWith openWith)
{
  if (!share || share->isInaccessible())
  {
    return;
  }

  switch (openWith)
  {
    case FileManager:
    {
      QUrl url = QUrl::fromLocalFile(share->canonicalPath());

      (void) new KRun(url, 0);

      break;
    }
    case Konsole:
    {
      QString konsole = QStandardPaths::findExecutable("konsole");

      if (konsole.isEmpty())
      {
        Smb4KNotification::commandNotFound("konsole");
      }
      else
      {
        KRun::runCommand(konsole+" --workdir "+KShell::quoteArg(share->canonicalPath()), 0);
      }

      break;
    }
    default:
    {
      break;
    }
  }
}
