#include "AccelerometerSensor.h"


AccelerometerSensor::AccelerometerSensor() : _accelerometer(Accelerometer::GetDefault())
{
}


AccelerometerSensor::~AccelerometerSensor()
{
}

double AccelerometerSensor::getX()
{
	return _accelerometer->GetCurrentReading()->AccelerationX;
}

double AccelerometerSensor::getY()
{
	return _accelerometer->GetCurrentReading()->AccelerationY;
}

double AccelerometerSensor::getZ()
{
	return _accelerometer->GetCurrentReading()->AccelerationZ;
}
