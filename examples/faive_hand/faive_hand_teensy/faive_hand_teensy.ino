#include "src/faive_hand.pb.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "pb.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  uint8_t buffer[128];
  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer)); // what does this do?

  faive_Hand hand = faive_Hand_init_default;
  faive_Finger thumb = faive_Finger_init_default;
  thumb.id = 1;
  faive_Sensor thumb_HE1 = faive_Sensor_init_default;
  thumb_HE1.id = 1;
  thumb_HE1.type = faive_Sensor_SensorType_HES;
  faive_Sensor_SensorData thumb_HE1_X = faive_Sensor_SensorData_init_default;
  thumb_HE1_X.axis = 1;
  thumb_HE1_X.value = 12.34;
  faive_Sensor thumb_FSR = faive_Sensor_init_default;
  thumb_FSR.id = 1;
  thumb_FSR.type = faive_Sensor_SensorType_FSR;
  faive_Sensor_SensorData thumb_FSR_X = faive_Sensor_SensorData_init_default;
  thumb_FSR_X.axis = 1;
  thumb_FSR_X.value = 543.21;

  bool status = pb_encode(&stream, faive_Finger_fields, &thumb);

  if (!status) {
    Serial.print("Encoding failed: ");
    Serial.println(PB_GET_ERROR(&stream));
    return;
  }

  Serial.print("Message: ");

  for(int i = 0; i<stream.bytes_written; i++){
    Serial.printf("%02X",buffer[i]);
  }

  // Serial.print("Person name: ");
  // Serial.println(person.name);
  Serial.print("Senstype: ");
  Serial.println(thumb_HE1.type);



  // tutorial_AddressBook address_book = tutorial_AddressBook_init_default;
  // tutorial_Person person = tutorial_Person_init_default;
  // // person.name = "John Doe";
  // person.id = 1234;

  // uint8_t buffer[128];
  // pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer)); // what does this do?

  // person.id = 1234;

  // bool status = pb_encode(&stream, tutorial_Person_fields, &person);

  // if (!status) {
  //   Serial.print("Encoding failed: ");
  //   Serial.println(PB_GET_ERROR(&stream));
  //   return;
  // }

  // Serial.print("Message: ");

  // for(int i = 0; i<stream.bytes_written; i++){
  //   Serial.printf("%02X",buffer[i]);
  // }

  // // Serial.print("Person name: ");
  // // Serial.println(person.name);
  // Serial.print("Person id: ");
  // Serial.println(person.id);

}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.print("Person name: ");
  // Serial.println(address_book.person.name);
  // Serial.print("Person id: ");
  // Serial.println(address_book.person.id);
}
