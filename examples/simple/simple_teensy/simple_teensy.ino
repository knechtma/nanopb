#include "src/simple.pb.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "pb.h"

//string encode decode to pb
bool encode_string(pb_ostream_t* stream, const pb_field_t* field, void* const* arg)
{
    const char* str = (const char*)(*arg);

    if (!pb_encode_tag_for_field(stream, field))
        return false;

    return pb_encode_string(stream, (uint8_t*)str, strlen(str));
}

uint8_t buffer[1024] = {'\0'};
pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer)); // what does this do?

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  stream.bytes_written = 0;
  SimpleMessage msg = SimpleMessage_init_default;
  msg.lucky_number = 42;

  bool status = pb_encode(&stream, SimpleMessage_fields, &msg);

  if (!status) {
    Serial.println(PB_GET_ERROR(&stream));
    return;
  }

  for(int i = 0; i < stream.bytes_written; i++) {
    Serial.printf("%02X",buffer[i]);
  }
  Serial.println();

  
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i<stream.bytes_written; i++) {
    Serial.printf("%02X",buffer[i]);
  }
  Serial.println();
  delay(1000);

}
