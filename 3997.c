void cql_server::response::write_int(int32_t n)
{
    auto u = htonl(n);
    auto *s = reinterpret_cast<const int8_t*>(&u);
    _body.write(bytes_view(s, sizeof(u)));
}