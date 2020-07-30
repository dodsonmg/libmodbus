#include "modbus/modbus-helpers.hpp"

extern "C" {
    #include "modbus-private.h"
}

/******************
 * HELPER FUNCTIONS
 *****************/

void
print_shim_info(std::string file, std::string function)
{
    int length = file.length() + function.length() + 3;
    std::cout << display_marker << std::endl;
    std::cout << file << ":" << function << "()" << std::endl;
    std::cout << std::string(length, '-') << std::endl;
}

// /**
//  * Print the name of a requested function
//  * */
// void
// print_modbus_function_name(int function)
// {
//     std::cout << std::showbase // show the 0x prefix
//             << std::internal // fill between the prefix and the number
//             << std::setfill('0') // fill with 0s
//             << std::hex;
//     std::cout << ">\tfunction: " << function << " (" <<
//         get_modbus_function_name(function) << ")" << std::endl;
// }

/**
 * gets the name of a modbus function
 *
 * modifies READ_COILS to READ_SINGLE_COIL or READ_MULTIPLE_COILS
 * */
std::string
modbus_get_function_name(modbus_t *ctx, const uint8_t *req)
{
    int *function = (int *)malloc(sizeof(int));
    int *offset = (int *)malloc(sizeof(int));
    int *slave_id = (int *)malloc(sizeof(int));
    uint16_t *addr = (uint16_t *)malloc(sizeof(uint16_t));
    int *nb = (int *)malloc(sizeof(int));
    uint16_t *addr_wr = (uint16_t *)malloc(sizeof(uint16_t));
    int *nb_wr = (int *)malloc(sizeof(int));

    modbus_decompose_request(ctx, req, offset, slave_id, function, addr, nb, addr_wr, nb_wr);
    std::string name = modbus_get_function_name(*function);

    /* distinguish between reading a single or multiple coils */
    if(name == "MODBUS_FC_READ_COILS") {
        if(*nb == 1) {
            name = "MODBUS_FC_READ_SINGLE_COIL";
        } else {
            name = "MODBUS_FC_READ_MULTIPLE_COILS";
        }
    }

    return name;
}

std::string
modbus_get_function_name(int function)
{
    switch(function) {
        case MODBUS_FC_READ_COILS:
            return "MODBUS_FC_READ_COILS";
        case MODBUS_FC_READ_DISCRETE_INPUTS:
            return "MODBUS_FC_READ_DISCRETE_INPUTS";
        case MODBUS_FC_READ_HOLDING_REGISTERS:
            return "MODBUS_FC_READ_HOLDING_REGISTERS";
        case MODBUS_FC_READ_INPUT_REGISTERS:
            return "MODBUS_FC_READ_INPUT_REGISTERS";
        case MODBUS_FC_WRITE_SINGLE_COIL:
            return "MODBUS_FC_WRITE_SINGLE_COIL";
        case MODBUS_FC_WRITE_SINGLE_REGISTER:
            return "MODBUS_FC_WRITE_SINGLE_REGISTER";
        case MODBUS_FC_WRITE_MULTIPLE_COILS:
            return "MODBUS_FC_WRITE_MULTIPLE_COILS";
        case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
            return "MODBUS_FC_WRITE_MULTIPLE_REGISTERS";
        case MODBUS_FC_REPORT_SLAVE_ID:
            return "MODBUS_FC_REPORT_SLAVE_ID";
        case MODBUS_FC_READ_EXCEPTION_STATUS:
            return "MODBUS_FC_READ_EXCEPTION_STATUS";
        case MODBUS_FC_MASK_WRITE_REGISTER:
            return "MODBUS_FC_MASK_WRITE_REGISTER";
        case MODBUS_FC_WRITE_AND_READ_REGISTERS:
            return "MODBUS_FC_WRITE_AND_READ_REGISTERS";
        case MODBUS_FC_READ_STRING:
            return "MODBUS_FC_READ_STRING";
        case MODBUS_FC_WRITE_STRING:
            return "MODBUS_FC_WRITE_STRING";
        default:
            return "ILLEGAL FUNCTION";
    }
}

/* Print CHERI capability details of mb_mapping pointers */
void
print_mb_mapping(modbus_mapping_t* mb_mapping)
{
    printf("mb_mapping:\t\t%#p\n", (void *)mb_mapping);
    printf("->tab_bits:\t\t%#p\n", (void *)mb_mapping->tab_bits);
    printf("->tab_input_bits:\t%#p\n", (void *)mb_mapping->tab_input_bits);
    printf("->tab_input_registers:\t%#p\n", (void *)mb_mapping->tab_input_registers);
    printf("->tab_registers:\t%#p\n", (void *)mb_mapping->tab_registers);
    printf("->tab_string:\t\t%#p\n", (void *)mb_mapping->tab_string);
}

/**
 * Get the function code.
 * */
int
modbus_get_function_code(modbus_t *ctx, const uint8_t *req)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    int offset = ctx->backend->header_length;
    int function = req[offset];

    return function;
}

/**
 * Decomposes the request.
 *
 * Obtains slave_id, function, address, and nb from the request.
 *
 * addr_wr and nb_wr are only used for MODBUS_FC_WRITE_AND_READ_REGISTERS
 * */
int
modbus_decompose_request(modbus_t *ctx, const uint8_t *req, int *offset,
                         int *slave, int *function, uint16_t *addr, int *nb,
                         uint16_t *addr_wr, int *nb_wr)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

    *offset = ctx->backend->header_length;
    *slave = req[*offset - 1];
    *function = req[*offset];
    *addr = (req[*offset + 1] << 8) + req[*offset + 2];
    *addr_wr = 0x0;
    *nb_wr = 0x0;

    /* Data are flushed on illegal number of values errors. */
    switch (*function) {
    case MODBUS_FC_READ_COILS:
    case MODBUS_FC_READ_DISCRETE_INPUTS:
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
    case MODBUS_FC_WRITE_STRING:
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    case MODBUS_FC_READ_INPUT_REGISTERS:
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
        *nb = (req[*offset + 3] << 8) + req[*offset + 4];
        break;
    case MODBUS_FC_WRITE_SINGLE_COIL:
    case MODBUS_FC_WRITE_SINGLE_REGISTER:
    case MODBUS_FC_MASK_WRITE_REGISTER:
        *nb = 1;
        break;
    case MODBUS_FC_WRITE_AND_READ_REGISTERS:
        *nb = (req[*offset + 3] << 8) + req[*offset + 4];
        *addr_wr = (req[*offset + 5] << 8) + req[*offset + 6];
        *nb_wr = (req[*offset + 7] << 8) + req[*offset + 8];
        break;
    default:
        *nb = 0;
        break;
    }

    return 0;
}

/* Helper function to print the elements of a request. */
void
print_modbus_decompose_request(modbus_t *ctx, const uint8_t *req)
{
    int *function = (int *)malloc(sizeof(int));
    int *offset = (int *)malloc(sizeof(int));
    int *slave_id = (int *)malloc(sizeof(int));
    uint16_t *addr = (uint16_t *)malloc(sizeof(uint16_t));
    int *nb = (int *)malloc(sizeof(int));
    uint16_t *addr_wr = (uint16_t *)malloc(sizeof(uint16_t));
    int *nb_wr = (int *)malloc(sizeof(int));

    modbus_decompose_request(ctx, req, offset, slave_id, function, addr, nb, addr_wr, nb_wr);

    std::cout << std::showbase // show the 0x prefix
              << std::internal // fill between the prefix and the number
              << std::setfill('0') // fill with 0s
              << std::hex;
    std::cout << "decompose request:" << std::endl;
    std::cout << "> " << "offset:\t\t" << *offset << std::endl;
    std::cout << "> " << "slave_id:\t\t" << *slave_id << std::endl;
    std::cout << "> " << "function:\t\t" << *function << " (" <<
        modbus_get_function_name(*function) << ")" << std::endl;
    std::cout << "> " << "addr:\t\t\t" << *addr << std::endl;
    std::cout << "> " << "nb:\t\t\t" << *nb << std::endl;
    std::cout << "> " << "addr_wr:\t\t" << *addr_wr << std::endl;
    std::cout << "> " << "nb_wr:\t\t" << *nb_wr << std::endl;
}