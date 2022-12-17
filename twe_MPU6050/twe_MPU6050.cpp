#include <TWELITE>
#include <MPU6050/MPU6050.h>

/*** function prototype */
void timer_1();

//
float dt = 0.1f;//s
MPU6050 MPU6050;

/*** setup procedure (run once at cold boot) */
void setup() {

	Timer0.begin(uint16_t(1.0/dt));

	Serial.begin(115200);

	Wire.begin(WIRE_CONF::WIRE_400KHZ,false);//SCL:14 SDA:17
}

void begin() {
	if(MPU6050.init()){
		Serial << "init OK" << mwx::crlf;
	}else{
		Serial << "init NG" << mwx::crlf;
	}
}

/*** loop procedure (called every event) */
void loop() {
	timer_1();
}

void timer_1() {
	if (Timer0.available()) {
		if(!MPU6050.update()){
			Serial << "NG" << mwx::crlf;
		}
		Serial << format("%f,%f,%f,%f,%f,%f,%f",MPU6050.gyr[0],MPU6050.gyr[1],MPU6050.gyr[2],MPU6050.acc[0],MPU6050.acc[1],MPU6050.acc[2],MPU6050.temp) << mwx::crlf;
	}
}

