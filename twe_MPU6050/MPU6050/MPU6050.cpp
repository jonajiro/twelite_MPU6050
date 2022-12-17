#include <TWELITE>
#include "MPU6050.h"

MPU6050::MPU6050() {
    
    acc[0] = 0.0;
    acc[1] = 0.0;
    acc[2] = 0.0;
    gyr[0] = 0.0;
    gyr[1] = 0.0;
    gyr[2] = 0.0;
    temp = 0.0;
};
bool MPU6050::init() {
	int stat;
	Wire.beginTransmission(DEV_ADDR);
	stat = Wire.endTransmission();
	if(stat != 0){
		return false;
	}
	// 初回の読み出し
	Wire.beginTransmission(DEV_ADDR);
	Wire.write(0x75);
	Wire.write(0x00);
	Wire.endTransmission();

	// 動作モードの読み出し
	Wire.beginTransmission(DEV_ADDR);
	Wire.write(0x6B);
	Wire.write(0x00);
	Wire.endTransmission();

	//Gyro初期設定
	Wire.beginTransmission(DEV_ADDR);
	Wire.write(0x1B);//Gyroレンジ設定レジスタ
	//0x00:±250deg/s
	//0x08:±500deg/s
	//0x10:±1000deg/s
	//0x18:±200deg/s
	Wire.write(0x08);//引数に設定値を入力
	Wire.endTransmission();

	//加速度センサー初期設定
	Wire.beginTransmission(DEV_ADDR);
	Wire.write(0x1C);//加速度レンジ設定レジスタ
	//0x00:±2g
	//0x08:±4g
	//0x10:±8g
	//0x18:±16g
	Wire.write(0x10);//引数に設定値を入力
	Wire.endTransmission();

	//LPF設定
	Wire.beginTransmission(DEV_ADDR);
	Wire.write(0x1A);
	Wire.write(0x03);//引数に設定値を入力(data sheet参照)
	Wire.endTransmission();

  return true;
}

bool MPU6050::update(void) {
		
	if (auto&& wrt = Wire.get_writer(DEV_ADDR)) {
		wrt << 0x3B;
	}

	int16_t axRaw, ayRaw, azRaw, gxRaw, gyRaw, gzRaw, Temperature;
	uint8_t au8data[14];
	int i;
	if (auto&& rdr = Wire.get_reader(DEV_ADDR, 14)) {
		for(i = 0;i<sizeof(au8data);i++){
			rdr >> au8data[i];
		}
	}
	axRaw = au8data[0] << 8 | au8data[1];
	ayRaw = au8data[2] << 8 | au8data[3];
	azRaw = au8data[4] << 8 | au8data[5];
	Temperature = au8data[6] << 8 | au8data[7];
	gxRaw = au8data[7] << 8 | au8data[9];
	gyRaw = au8data[10] << 8 | au8data[11];
	gzRaw = au8data[12] << 8 | au8data[13];
	// 加速度値を分解能で割って加速度(m/s2)に変換する
	acc[0] = axRaw / (4096.0f) * GRAV;
	acc[1] = ayRaw / (4096.0f) * GRAV;
	acc[2] = azRaw / (4096.0f) * GRAV;

	// 角速度値を分解能で割って角速度(rad/s)に変換する
	gyr[0] = gxRaw / (65.5f) * M_PI / 180.0f;
	gyr[1] = gyRaw / (65.5f) * M_PI / 180.0f;
	gyr[2] = gzRaw / (65.5f) * M_PI / 180.0f;

    temp = Temperature/340.0f + 36.53f;
    return true;
};
