#pragma once
#using <Windows.winmd>
using namespace Windows::Devices::Sensors;

class AccelerometerSensor
{
public:
	AccelerometerSensor();
	~AccelerometerSensor();

	double getX();
	double getY();
	double getZ();

private:
	Accelerometer^ _accelerometer;
};

