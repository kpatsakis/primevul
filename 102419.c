int32_t Parcel::readExceptionCode() const
{
    binder::Status status;
    status.readFromParcel(*this);
 return status.exceptionCode();
}
