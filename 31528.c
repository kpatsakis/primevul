Smb4KShare* Smb4KGlobal::findShareByPath(const QString &path)
{
  Smb4KShare *share = 0;

  mutex.lock();

  if (!path.isEmpty() && !p->mountedSharesList.isEmpty())
  {
    for (Smb4KShare *s : p->mountedSharesList)
    {
      if (QString::compare(s->path(), path, Qt::CaseInsensitive) == 0 ||
          QString::compare(s->canonicalPath(), path, Qt::CaseInsensitive) == 0)
      {
        share = s;
        break;
      }
      else
      {
      }
    }
  }
  else
  {
  }

  mutex.unlock();

  return share;
}
