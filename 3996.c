    fmt_visitor(uint8_t version, cql_server::response& response, bool skip_metadata)
        : _version{version}
        , _response{response}
        , _skip_metadata{skip_metadata}
    { }