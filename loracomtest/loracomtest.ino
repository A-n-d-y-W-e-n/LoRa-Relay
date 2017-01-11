#include "LDHT.h"

#define DHTPIN 7          // what pin we're connected to
#define DHTTYPE DHT22     // using DHT22 sensor
#define FAN_PORT 10
#define LED_PORT 6

LDHT dht(DHTPIN, DHTTYPE);
void serial_one_msg();


float tempC = 0.0, Humi = 0.0;
char temp_humi[8];
char readcharbuffer[20];
int readbuffersize;
char temp_input;

int serial_delay_count = 0;
int read_flag = 1;

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);

  dht.begin();
  pinMode(LED_PORT, OUTPUT);
  pinMode(FAN_PORT, OUTPUT);

  Serial.print(DHTTYPE);
  Serial.println("test!");
}

void loop(){

  Serial.println("Getting DHT temperature and humidity information.");
  while(dht.read()<=0){
  }
  if (dht.read()){
    tempC = dht.readTemperature();
    Humi = dht.readHumidity();

    Serial.println("------------------------------");
    Serial.print("Temperature Celcius = ");
    Serial.print(dht.readTemperature());
    Serial.println("C");

    Serial.print("Temperature Fahrenheit = ");
    Serial.print(dht.readTemperature(false));
    Serial.println("F");

    Serial.print("Humidity = ");
    Serial.print(dht.readHumidity());
    Serial.println("%");

    Serial.print("HeatIndex = ");
    Serial.print(dht.readHeatIndex(tempC, Humi));
    Serial.println("C");

    Serial.print("DewPoint = ");
    Serial.print(dht.readDewPoint(tempC, Humi));
    Serial.println("C");

    sprintf(temp_humi,"%05.2f", Humi);
    Serial.println(Humi);
    Serial.println(temp_humi);

    Serial.println("Ready to Send");
    Serial.print("AT+DTX=11,\"T");
    Serial.print(dht.readHeatIndex(tempC, Humi));
    Serial.print(temp_humi);
    Serial.println("\"");

    Serial1.print("AT+DTX=11,\"T");
    Serial1.print(dht.readHeatIndex(tempC, Humi));
    Serial1.print(temp_humi);
    Serial1.println("\"");

    //Serial1.println("AT+DTX=11,\"12345ABCdef\"");
  }


  Serial.println("things 1");
  serial_one_msg();
  Serial.println("End of Node response");
  delay(1000);

//Dectect there are dl packets or not
  while(Serial1.available()<=0){
    serial_delay_count++;
    delay(1000);
    if(serial_delay_count == 10){
      read_flag = 0;
      serial_delay_count = 0;
      break;
    }
  }

  if(read_flag == 0){
    Serial.println("No downlink Message available");
    read_flag = 1;
  }else{
    readbuffersize = Serial1.available();
    Serial.print(readbuffersize);
    while(readbuffersize){
      temp_input = Serial1.read();
      Serial.print(temp_input);
      readbuffersize--;
    }
    //Wait until io is Ready
    delay(5000);
    //receiving message
    Serial1.println("AT+DRX?");
  }


  //Wait until io is Ready
  delay(5000);


  while(Serial1.available()<=0){
    serial_delay_count++;
    delay(1000);
    if(serial_delay_count == 10){
      read_flag = 0;
      serial_delay_count = 0;
      break;
    }
  }

  if(read_flag == 0){
    read_flag = 1;
  }else{
    readbuffersize = Serial1.available();
    Serial.print("Data from serial1 size is: ");
    Serial.println(readbuffersize);
    while(readbuffersize){
      temp_input = Serial1.read();
      Serial.print(temp_input);
      readbuffersize--;
    }
  }

  delay(45000);

  Serial.println("TX relay done should show up things 5");
  serial_one_msg();
  delay(1000);

//  dl_msg();*/
}

void serial_one_msg(){
  String serial1_temp_reading = "";

  while(Serial1.available()<=0){
    serial_delay_count++;
    delay(1000);
    if(serial_delay_count == 10){
      read_flag = 0;
      serial_delay_count = 0;
      break;
    }
  }

  if(read_flag == 0){
    read_flag = 1;
  }else{
    readbuffersize = Serial1.available();
    Serial.print("Data from serial1 size is: ");
    Serial.println(readbuffersize);
    while(Serial1.available()){
      serial1_temp_reading = Serial1.readString();
      Serial.print(serial1_temp_reading);
    }
  }
}
