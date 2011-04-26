/*
 * Copyright © 2001-2010 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * This library implements the Modbus protocol.
 * http://libmodbus.org/
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#include <modbus/config.h>
#include <Modbus/modbus.h>
#include <modbus/modbus-private.h>

/* Internal use */
#define MSG_LENGTH_UNDEFINED -1

/* Exported version */
const unsigned int libmodbus_version_major = LIBMODBUS_VERSION_MAJOR;
const unsigned int libmodbus_version_minor = LIBMODBUS_VERSION_MINOR;
const unsigned int libmodbus_version_micro = LIBMODBUS_VERSION_MICRO;

/* Max between RTU and TCP max adu length (so TCP) */
#define MAX_MESSAGE_LENGTH 260

const char *modbus_strerror(int errnum) {
    switch (errnum) {
    case EMBXILFUN:
        return "Illegal function";
    case EMBXILADD:
        return "Illegal data address";
    case EMBXILVAL:
        return "Illegal data value";
    case EMBXSFAIL:
        return "Slave device or server failure";
    case EMBXACK:
        return "Acknowledge";
    case EMBXSBUSY:
        return "Slave device or server is busy";
    case EMBXNACK:
        return "Negative acknowledge";
    case EMBXMEMPAR:
        return "Memory parity error";
    case EMBXGPATH:
        return "Gateway path unavailable";
    case EMBXGTAR:
        return "Target device failed to respond";
    case EMBBADCRC:
        return "Invalid CRC";
    case EMBBADDATA:
        return "Invalid data";
    case EMBBADEXC:
        return "Invalid exception code";
    case EMBMDATA:
        return "Too many data";
    default:
        return strerror(errnum);
    }
}

void _error_print(modbus_t *ctx, const char *context)
{
    if (ctx->debug) {
        fprintf(stderr, "ERROR %s", modbus_strerror(errno));
        if (context != NULL) {
            fprintf(stderr, ": %s\n", context);
        } else {
            fprintf(stderr, "\n");
        }
    }
}

int modbus_flush(modbus_t *ctx)
{
    return ctx->backend->flush(ctx);
}

/* Computes the length of the expected response */
static unsigned int compute_response_length(modbus_t *ctx, uint8_t *req)
{
    int length;
    int offset;

    offset = ctx->backend->header_length;

    switch (req[offset]) {
    case _FC_READ_COILS:
    case _FC_READ_DISCRETE_INPUTS: {
        /* Header + nb values (code from write_bits) */
        int nb = (req[offset + 3] << 8) | req[offset + 4];
        length = 2 + (nb / 8) + ((nb % 8) ? 1 : 0);
    }
        break;
    case _FC_READ_AND_WRITE_REGISTERS:
    case _FC_READ_HOLDING_REGISTERS:
    case _FC_READ_INPUT_REGISTERS:
        /* Header + 2 * nb values */
        length = 2 + 2 * (req[offset + 3] << 8 | req[offset + 4]);
        break;
    case _FC_READ_EXCEPTION_STATUS:
        length = 3;
        break;
    case _FC_REPORT_SLAVE_ID:
        /* The response is device specific (the header provides the
           length) */
        return MSG_LENGTH_UNDEFINED;
    default:
        length = 5;
    }

    return length + offset + ctx->backend->checksum_length;
}

/* Sends a request/response */
static int send_msg(modbus_t *ctx, uint8_t *req, int req_length)
{
    int rc;
    int i;

    req_length = ctx->backend->send_msg_pre(req, req_length);

    if (ctx->debug) {
        for (i = 0; i < req_length; i++)
            printf("[%.2X]", req[i]);
        printf("\n");
    }

    /* In recovery mode, the write command will be issued until to be
       successful! Disabled by default. */
    do {
        rc = ctx->backend->send(ctx, req, req_length);
        if (rc == -1) {
            _error_print(ctx, NULL);
            if (ctx->error_recovery &&
                (errno == EBADF || errno == ECONNRESET || errno == EPIPE)) {
                modbus_close(ctx);
                modbus_connect(ctx);
            }
        }
    } while (ctx->error_recovery && rc == -1);

    if (rc > 0 && rc != req_length) {
        errno = EMBBADDATA;
        return -1;
    }

    return rc;
}

/*
    ---------- Request     Indication ----------
    | Client | ---------------------->| Server |
    ---------- Confirmation  Response ----------
*/

typedef enum {
    /* Request message on the server side */
    MSG_INDICATION,
    /* Request message on the client side */
    MSG_CONFIRMATION
} msg_type_t;

/* Computes the header length (to reach the real data) */
static uint8_t compute_header_length(int function, msg_type_t msg_type)
{
    int length;

    if (msg_type == MSG_CONFIRMATION) {
        if (function == _FC_REPORT_SLAVE_ID) {
            length = 1;
        } else {
            /* Should never happen, the other header lengths are precomputed */
            abort();
        }
    } else /* MSG_INDICATION */ {
        if (function <= _FC_WRITE_SINGLE_COIL ||
            function == _FC_WRITE_SINGLE_REGISTER) {
            length = 4;
        } else if (function == _FC_WRITE_MULTIPLE_COILS ||
                   function == _FC_WRITE_MULTIPLE_REGISTERS) {
            length = 5;
        } else if (function == _FC_READ_AND_WRITE_REGISTERS) {
            length = 9;
        } else {
            length = 0;
        }
    }
    return length;
}

/* Computes the length of the data to write in the request */
static int compute_data_length(modbus_t *ctx, uint8_t *msg)
{
    int function = msg[ctx->backend->header_length];
    int length;

    if (function == _FC_WRITE_MULTIPLE_COILS ||
        function == _FC_WRITE_MULTIPLE_REGISTERS) {
        length = msg[ctx->backend->header_length + 5];
    } else if (function == _FC_REPORT_SLAVE_ID) {
        length = msg[ctx->backend->header_length + 1];
    } else if (function == _FC_READ_AND_WRITE_REGISTERS) {
        length = msg[ctx->backend->header_length + 9];
    } else
        length = 0;

    length += ctx->backend->checksum_length;

    return length;
}


/* Waits a response from a modbus server or a request from a modbus client.
   This function blocks if there is no replies (3 timeouts).

   The argument msg_length_computed must be set to MSG_LENGTH_UNDEFINED if
   undefined.

   The function shall return the number of received characters and the received
   message in an array of uint8_t if successful. Otherwise it shall return -1
   and errno is set to one of the values defined below:
   - ECONNRESET
   - EMBBADDATA
   - EMBUNKEXC
   - ETIMEDOUT
   - read() or recv() error codes
*/

static int receive_msg(modbus_t *ctx, int msg_length_computed,
                       uint8_t *msg, msg_type_t msg_type)
{
    int s_rc;
    int read_rc;
    fd_set rfds;
    struct timeval tv;
    int length_to_read;
    uint8_t *p_msg;
    enum { FUNCTION, DATA, COMPLETE };
    int state;
    int msg_length = 0;

    if (ctx->debug) {
        if (msg_type == MSG_INDICATION) {
            printf("Waiting for a indication");
        } else {
            printf("Waiting for a confirmation");
        }

        if (msg_length_computed == MSG_LENGTH_UNDEFINED)
            printf("...\n");
        else
            printf(" (%d bytes)...\n", msg_length_computed);
    }

    /* Add a file descriptor to the set */
    FD_ZERO(&rfds);
    FD_SET(ctx->s, &rfds);

    if (msg_length_computed == MSG_LENGTH_UNDEFINED) {
        /* Wait for a message */
        tv.tv_sec = 60;
        tv.tv_usec = 0;

        /* The message length is undefined (request receiving) so we need to
         * analyse the message step by step.  At the first step, we want to
         * reach the function code because all packets contains this
         * information. */
        state = FUNCTION;
        msg_length_computed = ctx->backend->header_length + 1;
    } else {
        tv.tv_sec = ctx->timeout_begin.tv_sec;
        tv.tv_usec = ctx->timeout_begin.tv_usec;
        state = COMPLETE;
    }

    length_to_read = msg_length_computed;

    s_rc = ctx->backend->select(ctx, &rfds, &tv, msg_length_computed, msg_length);
    if (s_rc == -1) {
        return -1;
    }

    p_msg = msg;
    while (s_rc) {
        read_rc = ctx->backend->recv(ctx, p_msg, length_to_read);
        if (read_rc == 0) {
            errno = ECONNRESET;
            read_rc = -1;
        }

        if (read_rc == -1) {
            _error_print(ctx, "read");
            if (ctx->error_recovery && (errno == ECONNRESET ||
                                        errno == ECONNREFUSED)) {
                modbus_close(ctx);
                modbus_connect(ctx);
                /* Could be removed by previous calls */
                errno = ECONNRESET;
                return -1;
            }
            return -1;
        }

        /* Sums bytes received */
        msg_length += read_rc;

        /* Display the hex code of each character received */
        if (ctx->debug) {
            int i;
            for (i=0; i < read_rc; i++)
                printf("<%.2X>", p_msg[i]);
        }

        if (msg_length < msg_length_computed) {
            /* Message incomplete */
            length_to_read = msg_length_computed - msg_length;
        } else {
            switch (state) {
            case FUNCTION:
                /* Function code position */
                length_to_read = compute_header_length(
                    msg[ctx->backend->header_length],
                    msg_type);
                msg_length_computed += length_to_read;
                /* It's useless to check the value of
                   msg_length_computed in this case (only
                   defined values are used). */
                if (length_to_read != 0) {
                    state = DATA;
                    break;
                } /* else switch straight to DATA */
            case DATA:
                length_to_read = compute_data_length(ctx, msg);
                msg_length_computed += length_to_read;
                if (msg_length_computed > ctx->backend->max_adu_length) {
                    errno = EMBBADDATA;
                    _error_print(ctx, "too many data");
                    return -1;
                }
                state = COMPLETE;
                break;
            case COMPLETE:
                length_to_read = 0;
                break;
            }
        }

        /* Moves the pointer to receive other data */
        p_msg = &(p_msg[read_rc]);

        if (length_to_read > 0) {
            /* If no character at the buffer wait
               TIME_OUT_END_OF_TRAME before to generate an error. */
            tv.tv_sec = ctx->timeout_end.tv_sec;
            tv.tv_usec = ctx->timeout_end.tv_usec;

            s_rc = ctx->backend->select(ctx, &rfds, &tv, msg_length_computed, msg_length);
            if (s_rc == -1) {
                return -1;
            }
        } else {
            /* All chars are received */
            s_rc = FALSE;
        }
    }

    if (ctx->debug)
        printf("\n");

    return ctx->backend->check_integrity(ctx, msg, msg_length);
}

/* Receive the request from a modbus master, requires the socket file descriptor
   etablished with the master device in argument or -1 to use the internal one
   of modbus_t.

   The function shall return the request received and its byte length if
   successul. Otherwise, it shall return -1 and errno is set. */
int modbus_receive(modbus_t *ctx, int sockfd, uint8_t *req)
{
    if (sockfd != -1) {
        ctx->s = sockfd;
    }

    /* The length of the request to receive isn't known. */
    return receive_msg(ctx, MSG_LENGTH_UNDEFINED, req, MSG_INDICATION);
}

/* Receives the response and checks values.

   The function shall return the number of values (bits or words) and the
   response if successful. Otherwise, its shall return -1 and errno is set.

   Note: all functions used to send or receive data with modbus return
   these values. */
static int receive_msg_req(modbus_t *ctx, uint8_t *req, uint8_t *rsp)
{
    int rc;
    int rsp_length_computed;
    int offset = ctx->backend->header_length;

    rsp_length_computed = compute_response_length(ctx, req);
    rc = receive_msg(ctx, rsp_length_computed, rsp, MSG_CONFIRMATION);
    if (rc != -1) {
        /* GOOD RESPONSE */
        int req_nb_value;
        int rsp_nb_value;
        int function = rsp[offset];

        if (function != req[offset]) {
            if (ctx->debug) {
                fprintf(stderr,
                        "Received function not corresponding to the request (%d != %d)\n",
                        function, req[offset]);
            }
            errno = EMBBADDATA;
            return -1;
        }

        /* The number of values is returned if it's corresponding
         * to the request */
        switch (function) {
        case _FC_READ_COILS:
        case _FC_READ_DISCRETE_INPUTS:
            /* Read functions, 8 values in a byte (nb
             * of values in the request and byte count in
             * the response. */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            req_nb_value = (req_nb_value / 8) + ((req_nb_value % 8) ? 1 : 0);
            rsp_nb_value = rsp[offset + 1];
            break;
        case _FC_READ_AND_WRITE_REGISTERS:
        case _FC_READ_HOLDING_REGISTERS:
        case _FC_READ_INPUT_REGISTERS:
            /* Read functions 1 value = 2 bytes */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            rsp_nb_value = (rsp[offset + 1] / 2);
            break;
        case _FC_WRITE_MULTIPLE_COILS:
        case _FC_WRITE_MULTIPLE_REGISTERS:
            /* N Write functions */
            req_nb_value = (req[offset + 3] << 8) + req[offset + 4];
            rsp_nb_value = (rsp[offset + 3] << 8) | rsp[offset + 4];
            break;
        case _FC_REPORT_SLAVE_ID:
            /* Report slave ID (bytes received) */
            req_nb_value = rsp_nb_value = rsp[offset + 1];
            break;
        default:
            /* 1 Write functions & others */
            req_nb_value = rsp_nb_value = 1;
        }

        if (req_nb_value == rsp_nb_value) {
            rc = rsp_nb_value;
        } else {
            if (ctx->debug) {
                fprintf(stderr,
                        "Quantity not corresponding to the request (%d != %d)\n",
                        rsp_nb_value, req_nb_value);
            }
            errno = EMBBADDATA;
            rc = -1;
        }
    } else if (errno == EMBUNKEXC) {
        /* EXCEPTION CODE RECEIVED */

        /* CRC must be checked here (not done in receive_msg) */
        rc = ctx->backend->check_integrity(ctx, rsp,
                                           _MODBUS_EXCEPTION_RSP_LENGTH);
        if (rc == -1)
            return -1;

        /* Check for exception response.
           0x80 + function is stored in the exception
           response. */
        if (0x80 + req[offset] == rsp[offset]) {
            int exception_code = rsp[offset + 1];
            if (exception_code < MODBUS_EXCEPTION_MAX) {
                errno = MODBUS_ENOBASE + exception_code;
            } else {
                errno = EMBBADEXC;
            }
            _error_print(ctx, NULL);
            return -1;
        }
    }

    return rc;
}

static int response_io_status(int address, int nb,
                              uint8_t *tab_io_status,
                              uint8_t *rsp, int offset)
{
    int shift = 0;
    int byte = 0;
    int i;

    for (i = address; i < address+nb; i++) {
        byte |= tab_io_status[i] << shift;
        if (shift == 7) {
            /* Byte is full */
            rsp[offset++] = byte;
            byte = shift = 0;
        } else {
            shift++;
        }
    }

    if (shift != 0)
        rsp[offset++] = byte;

    return offset;
}

/* Build the exception response */
static int response_exception(modbus_t *ctx, sft_t *sft,
                              int exception_code, uint8_t *rsp)
{
    int rsp_length;

    sft->function = sft->function + 0x80;
    rsp_length = ctx->backend->build_response_basis(sft, rsp);

    /* Positive exception code */
    rsp[rsp_length++] = exception_code;

    return rsp_length;
}

/* Send a response to the receive request.
   Analyses the request and constructs a response.

   If an error occurs, this function construct the response
   accordingly.
*/
int modbus_reply(modbus_t *ctx, const uint8_t *req,
                 int req_length, modbus_mapping_t *mb_mapping)
{
    int offset = ctx->backend->header_length;
    int slave = req[offset - 1];
    int function = req[offset];
    uint16_t address = (req[offset + 1] << 8) + req[offset + 2];
    uint8_t rsp[MAX_MESSAGE_LENGTH];
    int rsp_length = 0;
    sft_t sft;

    if (ctx->backend->filter_request(ctx, slave) == 1) {
        /* Filtered */
        return 0;
    }

    sft.slave = slave;
    sft.function = function;
    sft.t_id = ctx->backend->prepare_response_tid(req, &req_length);

    switch (function) {
    case _FC_READ_COILS: {
        int nb = (req[offset + 3] << 8) + req[offset + 4];

        if ((address + nb) > mb_mapping->nb_bits) {
            if (ctx->debug) {
                fprintf(stderr, "Illegal data address %0X in read_bits\n",
                        address + nb);
            }
            rsp_length = response_exception(
                ctx, &sft,
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        } else {
            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            rsp[rsp_length++] = (nb / 8) + ((nb % 8) ? 1 : 0);
            rsp_length = response_io_status(address, nb,
                                            mb_mapping->tab_bits,
                                            rsp, rsp_length);
        }
    }
        break;
    case _FC_READ_DISCRETE_INPUTS: {
        /* Similar to coil status (but too many arguments to use a
         * function) */
        int nb = (req[offset + 3] << 8) + req[offset + 4];

        if ((address + nb) > mb_mapping->nb_input_bits) {
            if (ctx->debug) {
                fprintf(stderr, "Illegal data address %0X in read_input_bits\n",
                        address + nb);
            }
            rsp_length = response_exception(
                ctx, &sft,
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        } else {
            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            rsp[rsp_length++] = (nb / 8) + ((nb % 8) ? 1 : 0);
            rsp_length = response_io_status(address, nb,
                                            mb_mapping->tab_input_bits,
                                            rsp, rsp_length);
        }
    }
        break;
    case _FC_READ_HOLDING_REGISTERS: {
        int nb = (req[offset + 3] << 8) + req[offset + 4];

        if ((address + nb) > mb_mapping->nb_registers) {
            if (ctx->debug) {
                fprintf(stderr, "Illegal data address %0X in read_registers\n",
                        address + nb);
            }
            rsp_length = response_exception(
                ctx, &sft,
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        } else {
            int i;

            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            rsp[rsp_length++] = nb << 1;
            for (i = address; i < address + nb; i++) {
                rsp[rsp_length++] = mb_mapping->tab_registers[i] >> 8;
                rsp[rsp_length++] = mb_mapping->tab_registers[i] & 0xFF;
            }
        }
    }
        break;
    case _FC_READ_INPUT_REGISTERS: {
        /* Similar to holding registers (but too many arguments to use a
         * function) */
        int nb = (req[offset + 3] << 8) + req[offset + 4];

        if ((address + nb) > mb_mapping->nb_input_registers) {
            if (ctx->debug) {
                fprintf(stderr, "Illegal data address %0X in read_input_registers\n",
                        address + nb);
            }
            rsp_length = response_exception(
                ctx, &sft,
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        } else {
            int i;

            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            rsp[rsp_length++] = nb << 1;
            for (i = address; i < address + nb; i++) {
                rsp[rsp_length++] = mb_mapping->tab_input_registers[i] >> 8;
                rsp[rsp_length++] = mb_mapping->tab_input_registers[i] & 0xFF;
            }
        }
    }
        break;
    case _FC_WRITE_SINGLE_COIL:
        if (address >= mb_mapping->nb_bits) {
            if (ctx->debug) {
                fprintf(stderr, "Illegal data address %0X in write_bit\n",
                        address);
            }
            rsp_length = response_exception(
                ctx, &sft,
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        } else {
            int data = (req[offset + 3] << 8) + req[offset + 4];

            if (data == 0xFF00 || data == 0x0) {
                mb_mapping->tab_bits[address] = (data) ? ON : OFF;
                memcpy(rsp, req, req_length);
                rsp_length = req_length;
            } else {
                if (ctx->debug) {
                    fprintf(stderr,
                            "Illegal data value %0X in write_bit request at address %0X\n",
                            data, address);
                }
                rsp_length = response_exception(
                    ctx, &sft,
                    MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE, rsp);
            }
        }
        break;
    case _FC_WRITE_SINGLE_REGISTER:
        if (address >= mb_mapping->nb_registers) {
            if (ctx->debug) {
                fprintf(stderr, "Illegal data address %0X in write_register\n",
                        address);
            }
            rsp_length = response_exception(
                ctx, &sft,
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        } else {
            int data = (req[offset + 3] << 8) + req[offset + 4];

            mb_mapping->tab_registers[address] = data;
            memcpy(rsp, req, req_length);
            rsp_length = req_length;
        }
        break;
    case _FC_WRITE_MULTIPLE_COILS: {
        int nb = (req[offset + 3] << 8) + req[offset + 4];

        if ((address + nb) > mb_mapping->nb_bits) {
            if (ctx->debug) {
                fprintf(stderr, "Illegal data address %0X in write_bits\n",
                        address + nb);
            }
            rsp_length = response_exception(
                ctx, &sft,
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        } else {
            /* 6 = byte count */
            modbus_set_bits_from_bytes(mb_mapping->tab_bits, address, nb, &req[offset + 6]);

            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            /* 4 to copy the bit address (2) and the quantity of bits */
            memcpy(rsp + rsp_length, req + rsp_length, 4);
            rsp_length += 4;
        }
    }
        break;
    case _FC_WRITE_MULTIPLE_REGISTERS: {
        int nb = (req[offset + 3] << 8) + req[offset + 4];

        if ((address + nb) > mb_mapping->nb_registers) {
            if (ctx->debug) {
                fprintf(stderr, "Illegal data address %0X in write_registers\n",
                        address + nb);
            }
            rsp_length = response_exception(
                ctx, &sft,
                MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        } else {
            int i, j;
            for (i = address, j = 6; i < address + nb; i++, j += 2) {
                /* 6 and 7 = first value */
                mb_mapping->tab_registers[i] =
                    (req[offset + j] << 8) + req[offset + j + 1];
            }

            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            /* 4 to copy the address (2) and the no. of registers */
            memcpy(rsp + rsp_length, req + rsp_length, 4);
            rsp_length += 4;
        }
    }
        break;
    case _FC_REPORT_SLAVE_ID:
        rsp_length = ctx->backend->build_response_basis(&sft, rsp);
        /* 2 bytes */
        rsp[rsp_length++] = 2;
        rsp[rsp_length++] = ctx->slave;
        /* Slave is ON */
        rsp[rsp_length++] = 0xFF;
        break;
    case _FC_READ_EXCEPTION_STATUS:
        if (ctx->debug) {
            fprintf(stderr, "FIXME Not implemented\n");
        }
        errno = ENOPROTOOPT;
        return -1;
        break;

    case _FC_READ_AND_WRITE_REGISTERS: {
        int nb = (req[offset + 3] << 8) + req[offset + 4];
        uint16_t address_write = (req[offset + 5] << 8) + req[offset + 6];
        int nb_write = (req[offset + 7] << 8) + req[offset + 8];

        if ((address + nb) > mb_mapping->nb_registers ||
            (address_write + nb_write) > mb_mapping->nb_registers) {
            if (ctx->debug) {
                fprintf(stderr,
                        "Illegal data read address %0X or write address %0X in read_and_write_registers\n",
                        address + nb, address_write + nb_write);
            }
            rsp_length = response_exception(ctx, &sft,
                                            MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS, rsp);
        } else {
            int i, j;
            rsp_length = ctx->backend->build_response_basis(&sft, rsp);
            rsp[rsp_length++] = nb << 1;

            for (i = address; i < address + nb; i++) {
                rsp[rsp_length++] = mb_mapping->tab_registers[i] >> 8;
                rsp[rsp_length++] = mb_mapping->tab_registers[i] & 0xFF;
            }

            /* 10 and 11 = first value */
            for (i = address_write, j = 10; i < address_write + nb_write; i++, j += 2) {
                mb_mapping->tab_registers[i] =
                    (req[offset + j] << 8) + req[offset + j + 1];
            }
        }
    }
        break;

    default:
        rsp_length = response_exception(ctx, &sft,
                                        MODBUS_EXCEPTION_ILLEGAL_FUNCTION,
                                        rsp);
        break;
    }

    return send_msg(ctx, rsp, rsp_length);
}

/* Reads IO status */
static int read_io_status(modbus_t *ctx, int function,
                          int addr, int nb, uint8_t *data_dest)
{
    int rc;
    int req_length;

    uint8_t req[_MIN_REQ_LENGTH];
    uint8_t rsp[MAX_MESSAGE_LENGTH];

    req_length = ctx->backend->build_request_basis(ctx, function, addr, nb, req);

    rc = send_msg(ctx, req, req_length);
    if (rc > 0) {
        int i, temp, bit;
        int pos = 0;
        int offset;
        int offset_end;

        rc = receive_msg_req(ctx, req, rsp);
        if (rc == -1)
            return -1;

        offset = ctx->backend->header_length + 2;
        offset_end = offset + rc;
        for (i = offset; i < offset_end; i++) {
            /* Shift reg hi_byte to temp */
            temp = rsp[i];

            for (bit = 0x01; (bit & 0xff) && (pos < nb);) {
                data_dest[pos++] = (temp & bit) ? TRUE : FALSE;
                bit = bit << 1;
            }

        }
    }

    return rc;
}

/* Reads the boolean status of bits and sets the array elements
   in the destination to TRUE or FALSE (single bits). */
int modbus_read_bits(modbus_t *ctx, int addr, int nb, uint8_t *data_dest)
{
    int rc;

    if (nb > MODBUS_MAX_READ_BITS) {
        if (ctx->debug) {
            fprintf(stderr,
                    "ERROR Too many bits requested (%d > %d)\n",
                    nb, MODBUS_MAX_READ_BITS);
        }
        errno = EMBMDATA;
        return -1;
    }

    rc = read_io_status(ctx, _FC_READ_COILS, addr, nb, data_dest);

    if (rc == -1)
        return -1;
    else
        return nb;
}


/* Same as modbus_read_bits but reads the remote device input table */
int modbus_read_input_bits(modbus_t *ctx, int addr, int nb, uint8_t *data_dest)
{
    int rc;

    if (nb > MODBUS_MAX_READ_BITS) {
        if (ctx->debug) {
            fprintf(stderr,
                    "ERROR Too many discrete inputs requested (%d > %d)\n",
                    nb, MODBUS_MAX_READ_BITS);
        }
        errno = EMBMDATA;
        return -1;
    }

    rc = read_io_status(ctx, _FC_READ_DISCRETE_INPUTS, addr, nb, data_dest);

    if (rc == -1)
        return -1;
    else
        return nb;
}

/* Reads the data from a remove device and put that data into an array */
static int read_registers(modbus_t *ctx, int function, int addr, int nb,
                          uint16_t *data_dest)
{
    int rc;
    int req_length;
    uint8_t req[_MIN_REQ_LENGTH];
    uint8_t rsp[MAX_MESSAGE_LENGTH];

    if (nb > MODBUS_MAX_READ_REGISTERS) {
        if (ctx->debug) {
            fprintf(stderr,
                    "ERROR Too many registers requested (%d > %d)\n",
                    nb, MODBUS_MAX_READ_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }

    req_length = ctx->backend->build_request_basis(ctx, function, addr, nb, req);

    rc = send_msg(ctx, req, req_length);
    if (rc > 0) {
        int offset;
        int i;

        rc = receive_msg_req(ctx, req, rsp);
        if (rc == -1) {
            return -1;
        }

        offset = ctx->backend->header_length;

        for (i = 0; i < rc; i++) {
            /* shift reg hi_byte to temp OR with lo_byte */
            data_dest[i] = (rsp[offset + 2 + (i << 1)] << 8) |
                rsp[offset + 3 + (i << 1)];
        }
    }

    return rc;
}

/* Reads the holding registers of remote device and put the data into an
   array */
int modbus_read_registers(modbus_t *ctx, int addr, int nb, uint16_t *data_dest)
{
    int status;

    if (nb > MODBUS_MAX_READ_REGISTERS) {
        if (ctx->debug) {
            fprintf(stderr,
                    "ERROR Too many registers requested (%d > %d)\n",
                    nb, MODBUS_MAX_READ_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }

    status = read_registers(ctx, _FC_READ_HOLDING_REGISTERS,
                            addr, nb, data_dest);
    return status;
}

/* Reads the input registers of remote device and put the data into an array */
int modbus_read_input_registers(modbus_t *ctx, int addr, int nb,
                                uint16_t *data_dest)
{
    int status;

    if (nb > MODBUS_MAX_READ_REGISTERS) {
        fprintf(stderr,
                "ERROR Too many input registers requested (%d > %d)\n",
                nb, MODBUS_MAX_READ_REGISTERS);
        errno = EMBMDATA;
        return -1;
    }

    status = read_registers(ctx, _FC_READ_INPUT_REGISTERS,
                            addr, nb, data_dest);

    return status;
}

/* Write a value to the specified register of the remote device.
   Used by write_bit and write_register */
static int write_single(modbus_t *ctx, int function, int addr, int value)
{
    int rc;
    int req_length;
    uint8_t req[_MIN_REQ_LENGTH];

    req_length = ctx->backend->build_request_basis(ctx, function, addr, value, req);

    rc = send_msg(ctx, req, req_length);
    if (rc > 0) {
        /* Used by write_bit and write_register */
        uint8_t rsp[_MIN_REQ_LENGTH];
        rc = receive_msg_req(ctx, req, rsp);
    }

    return rc;
}

/* Turns ON or OFF a single bit of the remote device */
int modbus_write_bit(modbus_t *ctx, int addr, int state)
{
    int status;

    if (state)
        state = 0xFF00;

    status = write_single(ctx, _FC_WRITE_SINGLE_COIL, addr, state);

    return status;
}

/* Writes a value in one register of the remote device */
int modbus_write_register(modbus_t *ctx, int addr, int value)
{
    int status;

    status = write_single(ctx, _FC_WRITE_SINGLE_REGISTER, addr, value);

    return status;
}

/* Write the bits of the array in the remote device */
int modbus_write_bits(modbus_t *ctx, int addr, int nb, const uint8_t *data_src)
{
    int rc;
    int i;
    int byte_count;
    int req_length;
    int bit_check = 0;
    int pos = 0;

    uint8_t req[MAX_MESSAGE_LENGTH];

    if (nb > MODBUS_MAX_WRITE_BITS) {
        if (ctx->debug) {
            fprintf(stderr, "ERROR Writing too many bits (%d > %d)\n",
                    nb, MODBUS_MAX_WRITE_BITS);
        }
        errno = EMBMDATA;
        return -1;
    }

    req_length = ctx->backend->build_request_basis(ctx, _FC_WRITE_MULTIPLE_COILS,
                                                   addr, nb, req);
    byte_count = (nb / 8) + ((nb % 8) ? 1 : 0);
    req[req_length++] = byte_count;

    for (i = 0; i < byte_count; i++) {
        int bit;

        bit = 0x01;
        req[req_length] = 0;

        while ((bit & 0xFF) && (bit_check++ < nb)) {
            if (data_src[pos++])
                req[req_length] |= bit;
            else
                req[req_length] &=~ bit;

            bit = bit << 1;
        }
        req_length++;
    }

    rc = send_msg(ctx, req, req_length);
    if (rc > 0) {
        uint8_t rsp[MAX_MESSAGE_LENGTH];
        rc = receive_msg_req(ctx, req, rsp);
    }


    return rc;
}

/* Write the values from the array to the registers of the remote device */
int modbus_write_registers(modbus_t *ctx, int addr, int nb, const uint16_t *data_src)
{
    int rc;
    int i;
    int req_length;
    int byte_count;

    uint8_t req[MAX_MESSAGE_LENGTH];

    if (nb > MODBUS_MAX_WRITE_REGISTERS) {
        if (ctx->debug) {
            fprintf(stderr,
                    "ERROR Trying to write to too many registers (%d > %d)\n",
                    nb, MODBUS_MAX_WRITE_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }

    req_length = ctx->backend->build_request_basis(ctx,
                                                   _FC_WRITE_MULTIPLE_REGISTERS,
                                                   addr, nb, req);
    byte_count = nb * 2;
    req[req_length++] = byte_count;

    for (i = 0; i < nb; i++) {
        req[req_length++] = data_src[i] >> 8;
        req[req_length++] = data_src[i] & 0x00FF;
    }

    rc = send_msg(ctx, req, req_length);
    if (rc > 0) {
        uint8_t rsp[MAX_MESSAGE_LENGTH];
        rc = receive_msg_req(ctx, req, rsp);
    }

    return rc;
}

/* Read multiple registers from remote device to dest array and write multiple
   registers to remote device from data array. */
int modbus_read_and_write_registers(modbus_t *ctx,
                                    int read_addr, int read_nb, uint16_t *dest,
                                    int write_addr, int write_nb, const uint16_t *data)
{
    int rc;
    int req_length;
    int i;
    int byte_count;
    uint8_t req[MAX_MESSAGE_LENGTH];
    uint8_t rsp[MAX_MESSAGE_LENGTH];

    if (read_nb > MODBUS_MAX_READ_REGISTERS) {
        if (ctx->debug) {
            fprintf(stderr,
                    "ERROR Too many registers requested (%d > %d)\n",
                    read_nb, MODBUS_MAX_READ_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }

    if (write_nb > MODBUS_MAX_RW_WRITE_REGISTERS) {
        if (ctx->debug) {
            fprintf(stderr,
                    "ERROR Too many registers to write (%d > %d)\n",
                    write_nb, MODBUS_MAX_RW_WRITE_REGISTERS);
        }
        errno = EMBMDATA;
        return -1;
    }
    req_length = ctx->backend->build_request_basis(ctx,
                                                   _FC_READ_AND_WRITE_REGISTERS,
                                                   read_addr, read_nb, req);

    req[req_length++] = write_addr >> 8;
    req[req_length++] = write_addr & 0x00ff;
    req[req_length++] = write_nb >> 8;
    req[req_length++] = write_nb & 0x00ff;
    byte_count = write_nb * 2;
    req[req_length++] = byte_count;

    for (i = 0; i < write_nb; i++) {
        req[req_length++] = data[i] >> 8;
        req[req_length++] = data[i] & 0x00FF;
    }

    rc = send_msg(ctx, req, req_length);
    if (rc > 0) {
        int offset;

        rc = receive_msg_req(ctx, req, rsp);
        offset = ctx->backend->header_length;

        /* If rc is negative, the loop is jumped ! */
        for (i = 0; i < rc; i++) {
            /* shift reg hi_byte to temp OR with lo_byte */
            dest[i] = (rsp[offset + 2 + (i << 1)] << 8) |
                rsp[offset + 3 + (i << 1)];
        }
    }

    return rc;
}

/* Send a request to get the slave ID of the device (only available in serial
   communication). */
int modbus_report_slave_id(modbus_t *ctx, uint8_t *data_dest)
{
    int rc;
    int req_length;
    uint8_t req[_MIN_REQ_LENGTH];

    req_length = ctx->backend->build_request_basis(ctx, _FC_REPORT_SLAVE_ID,
                                                   0, 0, req);

    /* HACKISH, addr and count are not used */
    req_length -= 4;

    rc = send_msg(ctx, req, req_length);
    if (rc > 0) {
        int i;
        int offset;
        uint8_t rsp[MAX_MESSAGE_LENGTH];

        /* Byte count, slave id, run indicator status,
           additional data */
        rc = receive_msg_req(ctx, req, rsp);
        if (rc == -1)
            return -1;

        offset = ctx->backend->header_length + 2;

        for (i=0; i < rc; i++) {
            data_dest[i] = rsp[offset + i];
        }
    }

    return rc;
}

void _modbus_init_common(modbus_t *ctx)
{
    /* Slave and socket are initialized to -1 */
    ctx->slave = -1;
    ctx->s = -1;

    ctx->debug = FALSE;
    ctx->error_recovery = FALSE;

    ctx->timeout_begin.tv_sec = 0;
    ctx->timeout_begin.tv_usec = _TIME_OUT_BEGIN_OF_TRAME;

    ctx->timeout_end.tv_sec = 0;
    ctx->timeout_end.tv_usec = _TIME_OUT_END_OF_TRAME;
}

/* Define the slave number */
int modbus_set_slave(modbus_t *ctx, int slave)
{
    return ctx->backend->set_slave(ctx, slave);
}

/*
  When disabled (default), it is expected that the application will check for
  error returns and deal with them as necessary.

  It's not recommanded to enable error recovery for slave/server.

  When enabled, the library will attempt an immediate reconnection which may
  hang for several seconds if the network to the remote target unit is down.
  The write will try a infinite close/connect loop until to be successful and
  the select/read calls will just try to retablish the connection one time then
  will return an error (if the connecton was down, the values to read are
  certainly not available anymore after reconnection, except for slave/server).
*/
int modbus_set_error_recovery(modbus_t *ctx, int enabled)
{
    if (enabled == TRUE || enabled == FALSE) {
        ctx->error_recovery = (uint8_t) enabled;
    } else {
        errno = EINVAL;
        return -1;
    }

    return 0;
}

/* Get the timeout of begin of trame */
void modbus_get_timeout_begin(modbus_t *ctx, struct timeval *timeout)
{
    *timeout = ctx->timeout_begin;
}

/* Set timeout when waiting the beginning of a trame */
void modbus_set_timeout_begin(modbus_t *ctx, const struct timeval *timeout)
{
    ctx->timeout_begin = *timeout;
}

/* Get the timeout of end of trame */
void modbus_get_timeout_end(modbus_t *ctx, struct timeval *timeout)
{
    *timeout = ctx->timeout_end;
}

/* Set timeout when waiting the end of a trame */
void modbus_set_timeout_end(modbus_t *ctx, const struct timeval *timeout)
{
    ctx->timeout_end = *timeout;
}

int modbus_connect(modbus_t *ctx)
{
    return ctx->backend->connect(ctx);
}

/* Closes a  connection */
void modbus_close(modbus_t *ctx)
{
    if (ctx == NULL)
        return;

    ctx->backend->close(ctx);
}

/* Free an initialized modbus_t */
void modbus_free(modbus_t *ctx)
{
    if (ctx == NULL)
        return;

    free(ctx->backend_data);
    free(ctx);
}

/* Activates the debug messages */
void modbus_set_debug(modbus_t *ctx, int boolean)
{
    ctx->debug = boolean;
}

/* Allocates 4 arrays to store bits, input bits, registers and inputs
   registers. The pointers are stored in modbus_mapping structure.

   The modbus_mapping_new() function shall return the new allocated structure if
   successful. Otherwise it shall return NULL and set errno to ENOMEM. */
modbus_mapping_t* modbus_mapping_new(int nb_bits, int nb_input_bits,
                                     int nb_registers, int nb_input_registers)
{
    modbus_mapping_t *mb_mapping;

    mb_mapping = (modbus_mapping_t *)malloc(sizeof(modbus_mapping_t));
    if (mb_mapping == NULL) {
        return NULL;
    }

    /* 0X */
    mb_mapping->nb_bits = nb_bits;
    if (nb_bits == 0) {
        mb_mapping->tab_bits = NULL;
    } else {
        /* Negative number raises a POSIX error */
        mb_mapping->tab_bits =
            (uint8_t *) malloc(nb_bits * sizeof(uint8_t));
        if (mb_mapping->tab_bits == NULL) {
            free(mb_mapping);
            return NULL;
        }
        memset(mb_mapping->tab_bits, 0, nb_bits * sizeof(uint8_t));
    }

    /* 1X */
    mb_mapping->nb_input_bits = nb_input_bits;
    if (nb_input_bits == 0) {
        mb_mapping->tab_input_bits = NULL;
    } else {
        mb_mapping->tab_input_bits =
            (uint8_t *) malloc(nb_input_bits * sizeof(uint8_t));
        if (mb_mapping->tab_input_bits == NULL) {
            free(mb_mapping->tab_bits);
            free(mb_mapping);
            return NULL;
        }
        memset(mb_mapping->tab_input_bits, 0, nb_input_bits * sizeof(uint8_t));
    }

    /* 4X */
    mb_mapping->nb_registers = nb_registers;
    if (nb_registers == 0) {
        mb_mapping->tab_registers = NULL;
    } else {
        mb_mapping->tab_registers =
            (uint16_t *) malloc(nb_registers * sizeof(uint16_t));
        if (mb_mapping->tab_registers == NULL) {
            free(mb_mapping->tab_input_bits);
            free(mb_mapping->tab_bits);
            free(mb_mapping);
            return NULL;
        }
        memset(mb_mapping->tab_registers, 0, nb_registers * sizeof(uint16_t));
    }

    /* 3X */
    mb_mapping->nb_input_registers = nb_input_registers;
    if (nb_input_registers == 0) {
        mb_mapping->tab_input_registers = NULL;
    } else {
        mb_mapping->tab_input_registers =
            (uint16_t *) malloc(nb_input_registers * sizeof(uint16_t));
        if (mb_mapping->tab_input_registers == NULL) {
            free(mb_mapping->tab_registers);
            free(mb_mapping->tab_input_bits);
            free(mb_mapping->tab_bits);
            free(mb_mapping);
            return NULL;
        }
        memset(mb_mapping->tab_input_registers, 0,
               nb_input_registers * sizeof(uint16_t));
    }

    return mb_mapping;
}

/* Frees the 4 arrays */
void modbus_mapping_free(modbus_mapping_t *mb_mapping)
{
    free(mb_mapping->tab_input_registers);
    free(mb_mapping->tab_registers);
    free(mb_mapping->tab_input_bits);
    free(mb_mapping->tab_bits);
    free(mb_mapping);
}

#ifndef HAVE_STRLCPY
/*
 * Function strlcpy was originally developed by
 * Todd C. Miller <Todd.Miller@courtesan.com> to simplify writing secure code.
 * See ftp://ftp.openbsd.org/pub/OpenBSD/src/lib/libc/string/strlcpy.3
 * for more information.
 *
 * Thank you Ulrich Drepper... not!
 *
 * Copy src to string dest of size dest_size.  At most dest_size-1 characters
 * will be copied.  Always NUL terminates (unless dest_size == 0).  Returns
 * strlen(src); if retval >= dest_size, truncation occurred.
 */
size_t strlcpy(char *dest, const char *src, size_t dest_size)
{
    register char *d = dest;
    register const char *s = src;
    register size_t n = dest_size;

    /* Copy as many bytes as will fit */
    if (n != 0 && --n != 0) {
        do {
            if ((*d++ = *s++) == 0)
                break;
        } while (--n != 0);
    }

    /* Not enough room in dest, add NUL and traverse rest of src */
    if (n == 0) {
        if (dest_size != 0)
            *d = '\0'; /* NUL-terminate dest */
        while (*s++)
            ;
    }

    return (s - src - 1); /* count does not include NUL */
}
#endif
