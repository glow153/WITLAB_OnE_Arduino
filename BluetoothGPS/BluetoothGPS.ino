#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define BLUE_TX 10
#define BLUE_RX 11
#define GPS_TX 12
#define GPS_RX 13
#define GPS_BAUD 9600

TinyGPS gps;
SoftwareSerial gpsSrl(GPS_TX, GPS_RX);
SoftwareSerial bluetooth(BLUE_TX, BLUE_RX);

String getStrLatLng(){
    double latitude, longitude;
    long lLat, lLng;
    gps.get_position(&lLat,&lLng);
    latitude = (double) (lLat / 10000);
    longitude = (double) (lLng / 10000);
    String sLat(latitude);
    String sLng(longitude);
    return (sLat + "," + sLng);
}

void setup() {
    Serial.begin(9600);
    //initializing order is very important!!!!!!!
    bluetooth.begin(9600);
    gpsSrl.begin(GPS_BAUD);
}

void loop() {
    while(gpsSrl.available()){
        int c = gpsSrl.read();
        if(gps.encode(c)){
            String s = getStrLatLng();
            byte arr[s.length()+2];
            s.getBytes(arr, s.length()+1);
            arr[s.length()+1] = (byte)0x0A;//'\n'
            Serial.write(arr, s.length()+1);
            bluetooth.write(arr, s.length()+1);
        }
    }
}
