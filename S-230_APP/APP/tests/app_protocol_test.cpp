#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C"
{
#include "app_protocol.c"
}


int G2_SERVER_read_device_id_message_process(g2_server_packet_pt packet)
{
    g2_server_device_id_message_t pmsg = {0};
    pmsg.deviceID = APP_CONFIG_DeviceID();
    BSP_PROTOCOL_send_read_device_id_message(packet, &pmsg);
    return PROTOCOL_OK;
}



using namespace testing;
class Mock_FOO {
public:
    MOCK_METHOD1(set, int(int num));
    MOCK_METHOD0(get, int());
    MOCK_METHOD1(mock_c_func, int(c_struct_t *p_c_struct));
};
Mock_FOO mocker;

TEST(APP_CONFIG_DeviceID, pass)
{
}