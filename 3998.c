std::unique_ptr<cql_server::response> cql_server::connection::make_read_failure_error(int16_t stream, exceptions::exception_code err, sstring msg, db::consistency_level cl, int32_t received, int32_t numfailures, int32_t blockfor, bool data_present, const tracing::trace_state_ptr& tr_state) const
{
    if (_version < 4) {
        return make_read_timeout_error(stream, exceptions::exception_code::READ_TIMEOUT, std::move(msg), cl, received, blockfor, data_present, tr_state);
    }
    auto response = std::make_unique<cql_server::response>(stream, cql_binary_opcode::ERROR, tr_state);
    response->write_int(static_cast<int32_t>(err));
    response->write_string(msg);
    response->write_consistency(cl);
    response->write_int(received);
    response->write_int(blockfor);
    response->write_int(numfailures);
    response->write_byte(data_present);
    return response;
}