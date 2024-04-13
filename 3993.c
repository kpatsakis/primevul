void cql_server::response::compress(cql_compression compression)
{
    switch (compression) {
    case cql_compression::lz4:
        compress_lz4();
        break;
    case cql_compression::snappy:
        compress_snappy();
        break;
    default:
        throw std::invalid_argument("Invalid CQL compression algorithm");
    }
    set_frame_flag(cql_frame_flags::compression);
}