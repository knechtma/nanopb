#include "src/faive_hand.pb.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "pb.h"

#include <QNEthernet.h>
#include <QNEthernetUDP.h>

using namespace qindesign::network;

//string encode decode to pb
bool encode_string(pb_ostream_t* stream, const pb_field_t* field, void* const* arg)
{
    const char* str = (const char*)(*arg);

    if (!pb_encode_tag_for_field(stream, field))
        return false;

    return pb_encode_string(stream, (uint8_t*)str, strlen(str));
}


uint8_t buffer[1024] = {'\0'};
pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

IPAddress ip{192, 168, 0, 143};   // Unique IP
IPAddress sn{255,255,255,0};  // Subnet Mask
IPAddress gw{192, 168, 0, 1};       // Default Gateway

unsigned int localPort = 5000;      // local port to listen on
const int UDP_TX_PACKET_MAX_SIZE = 1024;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; // buffer to hold incoming packet
String datReq; // String to hold incoming packet
int packetSize; // Size of incoming packet
EthernetUDP Udp; // A UDP instance to let us send and receive packets over UDP




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (!Ethernet.begin(ip,sn,gw)) {
    Serial.println("Failed to start Ethernet\n");
    return;
  }
  Udp.begin(localPort);

  uint8_t mac[6];
  Ethernet.macAddress(mac);  // Retrieve the MAC address and print it out
  Serial.printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n",
         mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  
  // Show whether a cable is plugged in or not.
  Ethernet.onLinkState([](bool state) {
    Serial.printf("[Ethernet] Link %s\n", state ? "ON" : "OFF");
  });

  Serial.print("Teensy is at ");
  Serial.println(Ethernet.localIP());

  delay(1500);



  stream.bytes_written = 0;

  faive_Event Handdata = faive_Event_init_default;
  Handdata.type = faive_Event_EventType_HANDDATA;

  faive_Event Fingerdata = faive_Event_init_default;
  Fingerdata.type = faive_Event_EventType_FINGERDATA;

  Handdata.has_handdata = true;
  faive_Hand& hand = Handdata.handdata;
  hand = faive_Hand_init_default;

  Fingerdata.has_fingerdata = true;

  // hand.fingers[0] = thumb;

  // const char* THUMB_NAME_STR = "thumb";
  faive_Finger& thumb = hand.fingers[0];
  thumb = faive_Finger_init_default;
  thumb.type = faive_Finger_FingerType_THUMB;
  // thumb.name.arg = THUMB_NAME_STR;
  // thumb.name.funcs.encode = &encode_string;

  faive_Finger& index_finger = hand.fingers[1];
  index_finger = faive_Finger_init_default;
  index_finger.type = faive_Finger_FingerType_INDEX;
  // index_finger.name.arg = "index";
  // index_finger.name.funcs.encode = &encode_string;

  faive_Finger& middle_finger = hand.fingers[2];
  middle_finger = faive_Finger_init_default;
  middle_finger.type = faive_Finger_FingerType_MIDDLE;

  faive_Finger& ring_finger = hand.fingers[3];
  ring_finger = faive_Finger_init_default;
  ring_finger.type = faive_Finger_FingerType_RING;

  faive_Finger& pinky_finger = hand.fingers[4];
  pinky_finger = faive_Finger_init_default;
  pinky_finger.type = faive_Finger_FingerType_PINKY;

  faive_Sensor& thumb_HE1 = thumb.sensors[0];
  thumb_HE1 = faive_Sensor_init_default;
  thumb_HE1.joint = faive_Sensor_JointType_CMC;
  thumb_HE1.type = faive_Sensor_SensorType_HES;

  faive_Sensor_SensorData& thumb_HE1_X = thumb_HE1.data[0];
  thumb_HE1_X = faive_Sensor_SensorData_init_default;
  thumb_HE1_X.axis = faive_Sensor_SensorData_AxisType_X;
  thumb_HE1_X.value = 12.34;

  faive_Sensor& thumb_FSR = thumb.sensors[4];
  thumb_FSR = faive_Sensor_init_default;
  thumb_FSR.digit = faive_Sensor_DigitType_DP;
  thumb_FSR.type = faive_Sensor_SensorType_FSR;
  
  faive_Sensor_SensorData& thumb_FSR_N = thumb_FSR.data[0];
  thumb_FSR_N = faive_Sensor_SensorData_init_default;
  thumb_FSR_N.axis = faive_Sensor_SensorData_AxisType_NOAXI;
  thumb_FSR_N.value = 543.21;

  // bool status = pb_encode(&stream, faive_Event_fields, &Handdata);

  faive_Finger& thumb2 = Fingerdata.fingerdata;
  thumb2 = thumb;

  bool status = pb_encode(&stream, faive_Event_fields, &Fingerdata);

  if (!status) {
    Serial.print("Encoding failed: ");
    Serial.println(PB_GET_ERROR(&stream));
    return;
  }

  // Serial.print("Message: ");

//   Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
//   for(int i = 0; i<stream.bytes_written; i++){
//     Udp.write(buffer[i]);
//     // Serial.printf("%02X",buffer[i]);
//   }
//   // Serial.println();
//   Udp.endPacket();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Writing to UDP");

  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  for(int i = 0; i<stream.bytes_written; i++){
    Udp.write(buffer[i]);
    // Serial.printf("%02X",buffer[i]);
  }
  // Serial.println();
  Udp.endPacket();

  // for(int i = 0; i<stream.bytes_written; i++){
  //   Serial.printf("%02X",buffer[i]);
  // }
  // Serial.println();
  delay(100);

  // read data from UDP
  packetSize = Udp.parsePacket();
  if (packetSize > 0) {
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    String datReq(packetBuffer);
    Serial.println(datReq);
  }
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);

  delay(100);

  
}
