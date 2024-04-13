status_t Parcel::writeDouble(double val)
{
 union {
 double d;
 unsigned long long ll;
 } u;
    u.d = val;
 return writeAligned(u.ll);
}
