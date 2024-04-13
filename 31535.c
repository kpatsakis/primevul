QList<Smb4KHost *> Smb4KGlobal::workgroupMembers(Smb4KWorkgroup *workgroup)
{
  QList<Smb4KHost *> hosts;

  mutex.lock();
  
  for (Smb4KHost *h : p->hostsList)
  {
    if (QString::compare(h->workgroupName(), workgroup->workgroupName(), Qt::CaseInsensitive) == 0)
    {
      hosts += h;
    }
    else
    {
    }
  }

  mutex.unlock();

  return hosts;
}
