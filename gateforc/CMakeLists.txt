
PROJECT(gateforc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_definitions(-D NN_HAVE_WINDOWS)
    add_definitions(-D _CRT_SECURE_NO_WARNINGS)
    link_libraries(Ws2_32 Synchronization)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    add_definitions(-w)
    link_directories(/home/ubuntu/vcpkg/installed/x64-linux/lib)
    link_libraries(backtrace pthread ssl crypto z uring  sqlite3)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Windows")

include_directories(../inc ../../libsz/inc/os ../../libsz/configs ../../libsz/Utils ../../libsz/hotupdate ../../libsz/database ../../libsz/inc/${CMAKE_SYSTEM_NAME}/export_flags ../../libsz/networkinterface ../../libsz/inc)

AUX_SOURCE_DIRECTORY(. A)



add_executable(gateforc ${A} )

target_link_libraries(gateforc PRIVATE nng::nngpp PRIVATE toml11::toml11 PRIVATE tinyutf8::tinyutf8 PRIVATE unofficial::sqlite3::sqlite3 PRIVATE sqlpp11::sqlpp11 PRIVATE date::date date::date-tz PRIVATE ZLIB::ZLIB PRIVATE $<IF:$<TARGET_EXISTS:uv_a>,uv_a,uv>  PRIVATE Utils PRIVATE OpenSSL::SSL PRIVATE OpenSSL::Crypto PRIVATE configs PRIVATE database PRIVATE hotupdate PRIVATE networkinterface PRIVATE Taskflow::Taskflow PRIVATE nng::nng PRIVATE fmt::fmt PRIVATE efsw::efsw)


set_target_properties(gateforc PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/config")

set_target_properties(gateforc PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(gateforc PROPERTIES VS_GLOBAL_VcpkgEnabled true)
