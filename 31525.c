void Smb4KGlobal::clearSharesList()
{
  mutex.lock();

  while (!p->sharesList.isEmpty())
  {
    delete p->sharesList.takeFirst();
  }

  mutex.unlock();
}
