#ifndef _MODBUS_HELPERS_
#define _MODBUS_HELPERS_

#include <iostream>
#include <iomanip>

/* for Modbus */
extern "C" {
    #include "modbus.h"
}

/*********
 * GLOBALS
 ********/

const std::string display_marker(80, '*');

/******************
 * HELPER FUNCTIONS
 *****************/

void print_shim_info(std::string file, std::string function);
// void print_modbus_function_name(int function);
std::string modbus_get_function_name(modbus_t *ctx, const uint8_t *req);
std::string modbus_get_function_name(int function);
void print_mb_mapping(modbus_mapping_t* mb_mapping);
int modbus_get_function_code(modbus_t *ctx, const uint8_t *req);
int modbus_decompose_request(modbus_t *ctx, const uint8_t *req, int *offset,
                             int *slave, int *function, uint16_t *addr, int *nb,
                             uint16_t *addr_wr, int *nb_wr);
void print_modbus_decompose_request(modbus_t *ctx, const uint8_t *req);

#endif /* _MODBUS_HELPERS_ */
