void Smb4KMounter::abortAll()
{
  if (!QCoreApplication::closingDown())
  {
    QListIterator<KJob *> it(subjobs());
    
    while (it.hasNext())
    {
      it.next()->kill(KJob::EmitResult);
    }
  }
  else
  {
  }
}
