    virtual void visit(const messages::result_message::prepared::cql& m) override {
        _response.write_int(0x0004);
        _response.write_short_bytes(m.get_id());
        _response.write(m.metadata(), _version);
        if (_version > 1) {
            _response.write(*m.result_metadata());
        }
    }