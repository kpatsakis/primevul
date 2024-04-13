std::tuple<net::inet_address, int, client_type> cql_server::connection::make_client_key(const service::client_state& cli_state) {
    return std::make_tuple(cli_state.get_client_address().addr(),
            cli_state.get_client_port(),
            cli_state.is_thrift() ? client_type::thrift : client_type::cql);
}