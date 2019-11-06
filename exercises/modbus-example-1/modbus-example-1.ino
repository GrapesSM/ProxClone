#define UBRR1H _SFR_MEM8(0xCD)
#include<ModbusRtu.h>
#define led1 11
#define led2 12
#define led3 13

Modbus bus;
uint16_t modbus_array[] = {0, 0, 0};

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  bus = Modbus(1,1,4); 
  Serial.begin(9600);
  bus.begin(9600);
}

void loop() {
  bus.poll(modbus_array, sizeof(modbus_array) / sizeof(modbus_array[0]));
  for (int i = 0, n = sizeof(modbus_array) / sizeof(modbus_array[0]); i < n; i++) {
    Serial.print(modbus_array[i]);
    Serial.print(",");
  }
  Serial.println();

  if (modbus_array[0] == 0) {
    digitalWrite(led1, LOW);
  } else {
    digitalWrite(led1, HIGH);
  }

  if (modbus_array[1] == 0) {
    digitalWrite(led2, LOW);
  } else {
    digitalWrite(led2, HIGH);
  }

  if (modbus_array[2] == 0) {
    digitalWrite(led3, LOW);
  } else {
    digitalWrite(led3, HIGH);
  }
}
