#ifndef _MODBUS_HELPERS_H_
#define _MODBUS_HELPERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* for Modbus */
#include "modbus.h"

/*********
 * GLOBALS
 ********/

const char display_marker[] =
    "****************************************"
    "****************************************\n";

/******************
 * HELPER FUNCTIONS
 *****************/

void print_shim_info(const char *file, const char *function);
char* modbus_get_function_name(modbus_t *ctx, const uint8_t *req);
char* modbus_get_function_name_from_fc(int function);
void print_mb_mapping(modbus_mapping_t* mb_mapping);
int modbus_get_function_code(modbus_t *ctx, const uint8_t *req);
int modbus_decompose_request(modbus_t *ctx, const uint8_t *req, int *offset,
                             int *slave, int *function, uint16_t *addr, int *nb,
                             uint16_t *addr_wr, int *nb_wr);
void print_modbus_decompose_request(modbus_t *ctx, const uint8_t *req);

#endif /* _MODBUS_HELPERS_H_ */
