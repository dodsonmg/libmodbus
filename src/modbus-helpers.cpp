#include "modbus/modbus-helpers.hpp"

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

/**
 * Print the name of a requested function
 * */
void
print_modbus_function_name(int function)
{
    std::cout << std::showbase // show the 0x prefix
            << std::internal // fill between the prefix and the number
            << std::setfill('0') // fill with 0s
            << std::hex;
    std::cout << ">\tfunction: " << function << " (" <<
        get_modbus_function_name(function) << ")" << std::endl;
}

std::string
get_modbus_function_name(int function)
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

/* Helper function to print the elements of a request. */
void
print_modbus_decompose_request(modbus_t *ctx, const uint8_t *req, int *offset,
                               int *slave_id, int *function, uint16_t *addr, int *nb,
                               uint16_t *addr_wr, int *nb_wr)
{
    modbus_decompose_request(ctx, req, offset, slave_id, function, addr, nb, addr_wr, nb_wr);

    std::cout << std::showbase // show the 0x prefix
              << std::internal // fill between the prefix and the number
              << std::setfill('0') // fill with 0s
              << std::hex;
    std::cout << "decompose request:" << std::endl;
    std::cout << "> " << "offset:\t\t" << *offset << std::endl;
    std::cout << "> " << "slave_id:\t\t" << *slave_id << std::endl;
    std::cout << "> " << "function:\t\t" << *function << " (" <<
        get_modbus_function_name(*function) << ")" << std::endl;
    std::cout << "> " << "addr:\t\t\t" << *addr << std::endl;
    std::cout << "> " << "nb:\t\t\t" << *nb << std::endl;
    std::cout << "> " << "addr_wr:\t\t" << *addr_wr << std::endl;
    std::cout << "> " << "nb_wr:\t\t" << *nb_wr << std::endl;
}