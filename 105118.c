static void php_session_save_current_state(TSRMLS_D) /* {{{ */
{
	int ret = FAILURE;

	IF_SESSION_VARS() {
 		if (PS(mod_data) || PS(mod_user_implemented)) {
			char *val;
			int vallen;

			val = php_session_encode(&vallen TSRMLS_CC);
			if (val) {
				ret = PS(mod)->s_write(&PS(mod_data), PS(id), val, vallen TSRMLS_CC);
				efree(val);
			} else {
				ret = PS(mod)->s_write(&PS(mod_data), PS(id), "", 0 TSRMLS_CC);
			}
		}

		if (ret == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Failed to write session data (%s). Please "
					"verify that the current setting of session.save_path "
					"is correct (%s)",
					PS(mod)->s_name,
					PS(save_path));
		}
	}

	if (PS(mod_data) || PS(mod_user_implemented)) {
		PS(mod)->s_close(&PS(mod_data) TSRMLS_CC);
	}
}
/* }}} */
