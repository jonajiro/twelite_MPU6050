#ifndef _MPU6050_h_
#define _MPU6050_h_
#include <TWELITE>
#define GRAV 9.80665
#define M_PI 3.14159
#define DEV_ADDR 0x68
class MPU6050 {
public:
    MPU6050();
    bool init(void);
    bool update(void);

    float acc[3];
    float gyr[3];
    float temp;

private:
    
};

#endif
