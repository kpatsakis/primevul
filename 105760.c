MYSQLND_METHOD(mysqlnd_conn_data, simple_command_handle_response)(MYSQLND_CONN_DATA * conn, enum mysqlnd_packet_type ok_packet,
															 zend_bool silent, enum php_mysqlnd_server_command command,
															 zend_bool ignore_upsert_status TSRMLS_DC)
{
	enum_func_status ret = FAIL;

	DBG_ENTER("mysqlnd_conn_data::simple_command_handle_response");
	DBG_INF_FMT("silent=%u packet=%u command=%s", silent, ok_packet, mysqlnd_command_to_text[command]);

	switch (ok_packet) {
		case PROT_OK_PACKET:{
			MYSQLND_PACKET_OK * ok_response = conn->protocol->m.get_ok_packet(conn->protocol, FALSE TSRMLS_CC);
			if (!ok_response) {
				SET_OOM_ERROR(*conn->error_info);
				break;
			}
			if (FAIL == (ret = PACKET_READ(ok_response, conn))) {
				if (!silent) {
					DBG_ERR_FMT("Error while reading %s's OK packet", mysqlnd_command_to_text[command]);
					php_error_docref(NULL TSRMLS_CC, E_WARNING, "Error while reading %s's OK packet. PID=%u",
									 mysqlnd_command_to_text[command], getpid());
				}
			} else {
				DBG_INF_FMT("OK from server");
				if (0xFF == ok_response->field_count) {
					/* The server signalled error. Set the error */
					SET_CLIENT_ERROR(*conn->error_info, ok_response->error_no, ok_response->sqlstate, ok_response->error);
					ret = FAIL;
					/*
					  Cover a protocol design error: error packet does not
					  contain the server status. Therefore, the client has no way
					  to find out whether there are more result sets of
					  a multiple-result-set statement pending. Luckily, in 5.0 an
					  error always aborts execution of a statement, wherever it is
					  a multi-statement or a stored procedure, so it should be
					  safe to unconditionally turn off the flag here.
					*/
					conn->upsert_status->server_status &= ~SERVER_MORE_RESULTS_EXISTS;
					SET_ERROR_AFF_ROWS(conn);
				} else {
					SET_NEW_MESSAGE(conn->last_message, conn->last_message_len,
									ok_response->message, ok_response->message_len,
									conn->persistent);

					if (!ignore_upsert_status) {
						memset(conn->upsert_status, 0, sizeof(*conn->upsert_status));
						conn->upsert_status->warning_count = ok_response->warning_count;
						conn->upsert_status->server_status = ok_response->server_status;
						conn->upsert_status->affected_rows = ok_response->affected_rows;
						conn->upsert_status->last_insert_id = ok_response->last_insert_id;
					}
				}
			}
			PACKET_FREE(ok_response);
			break;
		}
		case PROT_EOF_PACKET:{
			MYSQLND_PACKET_EOF * ok_response = conn->protocol->m.get_eof_packet(conn->protocol, FALSE TSRMLS_CC);
			if (!ok_response) {
				SET_OOM_ERROR(*conn->error_info);
				break;
			}
			if (FAIL == (ret = PACKET_READ(ok_response, conn))) {
				SET_CLIENT_ERROR(*conn->error_info, CR_MALFORMED_PACKET, UNKNOWN_SQLSTATE,
								 "Malformed packet");
				if (!silent) {
					DBG_ERR_FMT("Error while reading %s's EOF packet", mysqlnd_command_to_text[command]);
					php_error_docref(NULL TSRMLS_CC, E_WARNING, "Error while reading %s's EOF packet. PID=%d",
									 mysqlnd_command_to_text[command], getpid());
				}
			} else if (0xFF == ok_response->field_count) {
				/* The server signalled error. Set the error */
				SET_CLIENT_ERROR(*conn->error_info, ok_response->error_no, ok_response->sqlstate, ok_response->error);
				SET_ERROR_AFF_ROWS(conn);
			} else if (0xFE != ok_response->field_count) {
				SET_CLIENT_ERROR(*conn->error_info, CR_MALFORMED_PACKET, UNKNOWN_SQLSTATE, "Malformed packet");
				if (!silent) {
					DBG_ERR_FMT("EOF packet expected, field count wasn't 0xFE but 0x%2X", ok_response->field_count);
					php_error_docref(NULL TSRMLS_CC, E_WARNING, "EOF packet expected, field count wasn't 0xFE but 0x%2X",
									ok_response->field_count);
				}
			} else {
				DBG_INF_FMT("OK from server");
			}
			PACKET_FREE(ok_response);
			break;
		}
		default:
			SET_CLIENT_ERROR(*conn->error_info, CR_MALFORMED_PACKET, UNKNOWN_SQLSTATE, "Malformed packet");
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Wrong response packet %u passed to the function", ok_packet);
			break;
	}
	DBG_INF(ret == PASS ? "PASS":"FAIL");
	DBG_RETURN(ret);
}
