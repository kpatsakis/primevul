bool Smb4KGlobal::addShare(Smb4KShare *share)
{
  Q_ASSERT(share);

  bool added = false;
  
  if (share)
  {
    mutex.lock();

    if (!findShare(share->unc(), share->workgroupName()))
    {
      p->sharesList.append(share);
      added = true;
    }
    else
    {
    }
  }
  else
  {
  }

  mutex.unlock();

  return added;
}
