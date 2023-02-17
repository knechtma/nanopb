#include "src/addressbook.pb.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "pb.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  uint8_t data[128] = {'\0'};
  tutorial_AddressBook address_book = tutorial_AddressBook_init_default;
  tutorial_Person person = tutorial_Person_init_default;
  // person.name = "John Doe";
  person.id = 1234;

  uint8_t buffer[128];
  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer)); // what does this do?

  person.id = 1234;

  bool status = pb_encode(&stream, tutorial_Person_fields, &person);

  if (!status) {
    Serial.print("Encoding failed: ");
    Serial.println(PB_GET_ERROR(&stream));
    return;
  }

  Serial.print("Message: ");

  for(int i = 0; i<stream.bytes_written; i++){
    Serial.printf("%02X",buffer[i]);
  }
  Serial.println();

  // Serial.print("Person name: ");
  // Serial.println(person.name);
  Serial.print("Person id: ");
  Serial.println(person.id);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.print("Person name: ");
  // Serial.println(address_book.person.name);
  // Serial.print("Person id: ");
  // Serial.println(address_book.person.id);
}
