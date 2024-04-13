MYSQLND_METHOD(mysqlnd_conn_data, simple_command)(MYSQLND_CONN_DATA * conn, enum php_mysqlnd_server_command command,
			   const zend_uchar * const arg, size_t arg_len, enum mysqlnd_packet_type ok_packet, zend_bool silent,
			   zend_bool ignore_upsert_status TSRMLS_DC)
{
	enum_func_status ret = PASS;
	MYSQLND_PACKET_COMMAND * cmd_packet;

	DBG_ENTER("mysqlnd_conn_data::simple_command");
	DBG_INF_FMT("command=%s ok_packet=%u silent=%u", mysqlnd_command_to_text[command], ok_packet, silent);
	DBG_INF_FMT("conn->server_status=%u", conn->upsert_status->server_status);

	switch (CONN_GET_STATE(conn)) {
		case CONN_READY:
			break;
		case CONN_QUIT_SENT:
			SET_CLIENT_ERROR(*conn->error_info, CR_SERVER_GONE_ERROR, UNKNOWN_SQLSTATE, mysqlnd_server_gone);
			DBG_ERR("Server is gone");
			DBG_RETURN(FAIL);
		default:
			SET_CLIENT_ERROR(*conn->error_info, CR_COMMANDS_OUT_OF_SYNC, UNKNOWN_SQLSTATE, mysqlnd_out_of_sync);
			DBG_ERR_FMT("Command out of sync. State=%u", CONN_GET_STATE(conn));
			DBG_RETURN(FAIL);
	}

	SET_ERROR_AFF_ROWS(conn);
	SET_EMPTY_ERROR(*conn->error_info);

	cmd_packet = conn->protocol->m.get_command_packet(conn->protocol, FALSE TSRMLS_CC);
	if (!cmd_packet) {
		SET_OOM_ERROR(*conn->error_info);
		DBG_RETURN(FAIL);
	}

	cmd_packet->command = command;
	if (arg && arg_len) {
		cmd_packet->argument = arg;
		cmd_packet->arg_len  = arg_len;
	}

	MYSQLND_INC_CONN_STATISTIC(conn->stats, STAT_COM_QUIT + command - 1 /* because of COM_SLEEP */ );

	if (! PACKET_WRITE(cmd_packet, conn)) {
		if (!silent) {
			DBG_ERR_FMT("Error while sending %s packet", mysqlnd_command_to_text[command]);
			php_error(E_WARNING, "Error while sending %s packet. PID=%d", mysqlnd_command_to_text[command], getpid());
		}
		CONN_SET_STATE(conn, CONN_QUIT_SENT);
		DBG_ERR("Server is gone");
		ret = FAIL;
	} else if (ok_packet != PROT_LAST) {
		ret = conn->m->simple_command_handle_response(conn, ok_packet, silent, command, ignore_upsert_status TSRMLS_CC);
	}

	PACKET_FREE(cmd_packet);
	DBG_INF(ret == PASS ? "PASS":"FAIL");
	DBG_RETURN(ret);
}
