static uint32_t ssi_sd_transfer(SSISlave *dev, uint32_t val)
{
    ssi_sd_state *s = FROM_SSI_SLAVE(ssi_sd_state, dev);

    /* Special case: allow CMD12 (STOP TRANSMISSION) while reading data.  */
    if (s->mode == SSI_SD_DATA_READ && val == 0x4d) {
        s->mode = SSI_SD_CMD;
        /* There must be at least one byte delay before the card responds.  */
        s->stopping = 1;
    }

    switch (s->mode) {
    case SSI_SD_CMD:
        if (val == 0xff) {
            DPRINTF("NULL command\n");
            return 0xff;
        }
        s->cmd = val & 0x3f;
        s->mode = SSI_SD_CMDARG;
        s->arglen = 0;
        return 0xff;
    case SSI_SD_CMDARG:
        if (s->arglen == 4) {
            SDRequest request;
            uint8_t longresp[16];
            /* FIXME: Check CRC.  */
            request.cmd = s->cmd;
            request.arg = (s->cmdarg[0] << 24) | (s->cmdarg[1] << 16)
                           | (s->cmdarg[2] << 8) | s->cmdarg[3];
            DPRINTF("CMD%d arg 0x%08x\n", s->cmd, request.arg);
            s->arglen = sd_do_command(s->sd, &request, longresp);
            if (s->arglen <= 0) {
                s->arglen = 1;
                s->response[0] = 4;
                DPRINTF("SD command failed\n");
            } else if (s->cmd == 58) {
                /* CMD58 returns R3 response (OCR)  */
                DPRINTF("Returned OCR\n");
                s->arglen = 5;
                s->response[0] = 1;
                memcpy(&s->response[1], longresp, 4);
            } else if (s->arglen != 4) {
                BADF("Unexpected response to cmd %d\n", s->cmd);
                /* Illegal command is about as near as we can get.  */
                s->arglen = 1;
                s->response[0] = 4;
            } else {
                /* All other commands return status.  */
                uint32_t cardstatus;
                uint16_t status;
                /* CMD13 returns a 2-byte statuse work. Other commands
                   only return the first byte.  */
                s->arglen = (s->cmd == 13) ? 2 : 1;
                cardstatus = (longresp[0] << 24) | (longresp[1] << 16)
                             | (longresp[2] << 8) | longresp[3];
                status = 0;
                if (((cardstatus >> 9) & 0xf) < 4)
                    status |= SSI_SDR_IDLE;
                if (cardstatus & ERASE_RESET)
                    status |= SSI_SDR_ERASE_RESET;
                if (cardstatus & ILLEGAL_COMMAND)
                    status |= SSI_SDR_ILLEGAL_COMMAND;
                if (cardstatus & COM_CRC_ERROR)
                    status |= SSI_SDR_COM_CRC_ERROR;
                if (cardstatus & ERASE_SEQ_ERROR)
                    status |= SSI_SDR_ERASE_SEQ_ERROR;
                if (cardstatus & ADDRESS_ERROR)
                    status |= SSI_SDR_ADDRESS_ERROR;
                if (cardstatus & CARD_IS_LOCKED)
                    status |= SSI_SDR_LOCKED;
                if (cardstatus & (LOCK_UNLOCK_FAILED | WP_ERASE_SKIP))
                    status |= SSI_SDR_WP_ERASE;
                if (cardstatus & SD_ERROR)
                    status |= SSI_SDR_ERROR;
                if (cardstatus & CC_ERROR)
                    status |= SSI_SDR_CC_ERROR;
                if (cardstatus & CARD_ECC_FAILED)
                    status |= SSI_SDR_ECC_FAILED;
                if (cardstatus & WP_VIOLATION)
                    status |= SSI_SDR_WP_VIOLATION;
                if (cardstatus & ERASE_PARAM)
                    status |= SSI_SDR_ERASE_PARAM;
                if (cardstatus & (OUT_OF_RANGE | CID_CSD_OVERWRITE))
                    status |= SSI_SDR_OUT_OF_RANGE;
                /* ??? Don't know what Parameter Error really means, so
                   assume it's set if the second byte is nonzero.  */
                if (status & 0xff)
                    status |= SSI_SDR_PARAMETER_ERROR;
                s->response[0] = status >> 8;
                s->response[1] = status;
                DPRINTF("Card status 0x%02x\n", status);
            }
            s->mode = SSI_SD_RESPONSE;
            s->response_pos = 0;
        } else {
            s->cmdarg[s->arglen++] = val;
        }
        return 0xff;
    case SSI_SD_RESPONSE:
        if (s->stopping) {
            s->stopping = 0;
            return 0xff;
        }
        if (s->response_pos < s->arglen) {
            DPRINTF("Response 0x%02x\n", s->response[s->response_pos]);
            return s->response[s->response_pos++];
        }
        if (sd_data_ready(s->sd)) {
            DPRINTF("Data read\n");
            s->mode = SSI_SD_DATA_START;
        } else {
            DPRINTF("End of command\n");
            s->mode = SSI_SD_CMD;
        }
        return 0xff;
    case SSI_SD_DATA_START:
        DPRINTF("Start read block\n");
        s->mode = SSI_SD_DATA_READ;
        return 0xfe;
    case SSI_SD_DATA_READ:
        val = sd_read_data(s->sd);
        if (!sd_data_ready(s->sd)) {
            DPRINTF("Data read end\n");
            s->mode = SSI_SD_CMD;
        }
        return val;
    }
    /* Should never happen.  */
    return 0xff;
}
