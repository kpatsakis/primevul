future<> cql_server::connection::process_request() {
    return read_frame().then_wrapped([this] (future<std::optional<cql_binary_frame_v3>>&& v) {
        auto maybe_frame = v.get0();
        if (!maybe_frame) {
            // eof
            return make_ready_future<>();
        }

        auto& f = *maybe_frame;

        const bool allow_shedding = _client_state.get_workload_type() == service::client_state::workload_type::interactive;
        if (allow_shedding && _shed_incoming_requests) {
            ++_server._stats.requests_shed;
            return _read_buf.skip(f.length).then([this, stream = f.stream] {
                write_response(make_error(stream, exceptions::exception_code::OVERLOADED,
                        "request shed due to coordinator overload", tracing::trace_state_ptr()));
                return make_ready_future<>();
            });
        }

        tracing_request_type tracing_requested = tracing_request_type::not_requested;
        if (f.flags & cql_frame_flags::tracing) {
            // If tracing is requested for a specific CQL command - flush
            // tracing info right after the command is over.
            tracing_requested = tracing_request_type::write_on_close;
        } else if (tracing::tracing::get_local_tracing_instance().trace_next_query()) {
            tracing_requested = tracing_request_type::no_write_on_close;
        }

        auto op = f.opcode;
        auto stream = f.stream;
        auto mem_estimate = f.length * 2 + 8000; // Allow for extra copies and bookkeeping
        if (mem_estimate > _server._max_request_size) {
            write_response(make_error(stream, exceptions::exception_code::INVALID,
                    format("request size too large (frame size {:d}; estimate {:d}; allowed {:d}", f.length, mem_estimate, _server._max_request_size),
                    tracing::trace_state_ptr()));
            return std::exchange(_ready_to_respond, make_ready_future<>()).then([this] {
                return _read_buf.close();
            });
        }

        if (_server._stats.requests_serving > _server._max_concurrent_requests) {
            ++_server._stats.requests_shed;
            return _read_buf.skip(f.length).then([this, stream = f.stream] {
                write_response(make_error(stream, exceptions::exception_code::OVERLOADED,
                        format("too many in-flight requests (configured via max_concurrent_requests_per_shard): {}", _server._stats.requests_serving),
                        tracing::trace_state_ptr()));
                return make_ready_future<>();
            });
        }

        const auto shedding_timeout = std::chrono::milliseconds(50);
        auto fut = allow_shedding
                ? get_units(_server._memory_available, mem_estimate, shedding_timeout).then_wrapped([this, length = f.length] (auto f) {
                    try {
                        return make_ready_future<semaphore_units<>>(f.get0());
                    } catch (semaphore_timed_out sto) {
                        // Cancel shedding in case no more requests are going to do that on completion
                        if (_pending_requests_gate.get_count() == 0) {
                            _shed_incoming_requests = false;
                        }
                        return _read_buf.skip(length).then([sto = std::move(sto)] () mutable {
                            return make_exception_future<semaphore_units<>>(std::move(sto));
                        });
                    }
                })
                : get_units(_server._memory_available, mem_estimate);
        if (_server._memory_available.waiters()) {
            if (allow_shedding && !_shedding_timer.armed()) {
                _shedding_timer.arm(shedding_timeout);
            }
            ++_server._stats.requests_blocked_memory;
        }

        return fut.then_wrapped([this, length = f.length, flags = f.flags, op, stream, tracing_requested] (auto mem_permit_fut) {
          if (mem_permit_fut.failed()) {
              // Ignore semaphore errors - they are expected if load shedding took place
              mem_permit_fut.ignore_ready_future();
              return make_ready_future<>();
          }
          semaphore_units<> mem_permit = mem_permit_fut.get0();
          return this->read_and_decompress_frame(length, flags).then([this, op, stream, tracing_requested, mem_permit = make_service_permit(std::move(mem_permit))] (fragmented_temporary_buffer buf) mutable {

            ++_server._stats.requests_served;
            ++_server._stats.requests_serving;

            _pending_requests_gate.enter();
            auto leave = defer([this] {
                _shedding_timer.cancel();
                _shed_incoming_requests = false;
                _pending_requests_gate.leave();
            });
            auto istream = buf.get_istream();
            (void)_process_request_stage(this, istream, op, stream, seastar::ref(_client_state), tracing_requested, mem_permit)
                    .then_wrapped([this, buf = std::move(buf), mem_permit, leave = std::move(leave)] (future<foreign_ptr<std::unique_ptr<cql_server::response>>> response_f) mutable {
                try {
                    write_response(response_f.get0(), std::move(mem_permit), _compression);
                    _ready_to_respond = _ready_to_respond.finally([leave = std::move(leave)] {});
                } catch (...) {
                    clogger.error("request processing failed: {}", std::current_exception());
                }
            });

            return make_ready_future<>();
          });
        });
    });
}