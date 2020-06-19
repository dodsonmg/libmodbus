cmake_minimum_required(VERSION 3.5)
project(libmodbus)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)

add_library(modbus SHARED
            src/modbus.c
            src/modbus-data.c
            src/modbus-rtu.c
            src/modbus-tcp.c
            )

target_include_directories(modbus PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

ament_export_interfaces(export_modbus HAS_LIBRARY_TARGET)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  TARGETS modbus
  EXPORT export_modbus
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

# build unit tests for client and server
add_executable(unit_test_client
  tests/unit-test-client.c)
target_link_libraries(unit_test_client modbus)
ament_target_dependencies(unit_test_client modbus)

install(TARGETS
  unit_test_client
  DESTINATION lib/${PROJECT_NAME})

add_executable(unit_test_server
  tests/unit-test-server.c)
  target_link_libraries(unit_test_server modbus)
  ament_target_dependencies(unit_test_server modbus)

  install(TARGETS
    unit_test_server
    DESTINATION lib/${PROJECT_NAME})

ament_export_include_directories(include)
ament_export_libraries(modbus)

ament_package()