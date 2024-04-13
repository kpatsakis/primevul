void cql_server::response::compress_lz4()
{
    using namespace compression_buffers;
    auto view = input_buffer.get_linearized_view(_body);
    const char* input = reinterpret_cast<const char*>(view.data());
    size_t input_len = view.size();

    size_t output_len = LZ4_COMPRESSBOUND(input_len) + 4;
    _body = output_buffer.make_buffer(output_len, [&] (bytes_mutable_view output_view) {
        char* output = reinterpret_cast<char*>(output_view.data());
        output[0] = (input_len >> 24) & 0xFF;
        output[1] = (input_len >> 16) & 0xFF;
        output[2] = (input_len >> 8) & 0xFF;
        output[3] = input_len & 0xFF;
#ifdef HAVE_LZ4_COMPRESS_DEFAULT
        auto ret = LZ4_compress_default(input, output + 4, input_len, LZ4_compressBound(input_len));
#else
        auto ret = LZ4_compress(input, output + 4, input_len);
#endif
        if (ret == 0) {
            throw std::runtime_error("CQL frame LZ4 compression failure");
        }
        return ret + 4;
    });
    on_compression_buffer_use();
}