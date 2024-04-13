status_t IPCThreadState::clearLastError()
{
 const status_t err = mLastError;
    mLastError = NO_ERROR;
 return err;
}
