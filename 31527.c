Smb4KShare *Smb4KGlobal::findShare(const QString& unc, const QString& workgroup)
{
  Smb4KShare *share = 0;
  
  mutex.lock();

  for (Smb4KShare *s : p->sharesList)
  {
    if (QString::compare(s->unc(), unc, Qt::CaseInsensitive) == 0 &&
        (workgroup.isEmpty() || QString::compare(s->workgroupName(), workgroup, Qt::CaseInsensitive) == 0))
    {
      share = s;
      break;
    }
    else
    {
    }
  }
  
  mutex.unlock();
  
  return share;
}
