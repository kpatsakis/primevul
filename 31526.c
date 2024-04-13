QList<Smb4KShare*> Smb4KGlobal::findInaccessibleShares()
{
  QList<Smb4KShare *> inaccessibleShares;

  mutex.lock();
  
  for (Smb4KShare *s : p->mountedSharesList)
  {
    if (s->isInaccessible())
    {
      inaccessibleShares += s;
    }
    else
    {
    }
  }

  mutex.unlock();

  return inaccessibleShares;
}
