#include "pch.h"
#include <iostream>

//фейковые функции, чтобы во время написания кода их вызовы не подсвечивались как ошибка

const int8_t defaultSpeed = 5;

float get_dist(int8_t nOfSonar)
{
	return 0;
}

void car_right_fi(int8_t angle)
{

}
//конец фейковых функций


class RobotCar
{
public:
	RobotCar();
	RobotCar(int8_t newSpeed);
	int8_t ReturnSideOfObstacle();
private:
	 int8_t speed;
	float right;
	float center;
	float left;
	void _UpdateSensors();
};
RobotCar::RobotCar()
{
	speed = defaultSpeed;
}
RobotCar::RobotCar(int8_t newspeed)
{

}
void RobotCar::_UpdateSensors()
{
	left = get_dist(1);
	center = get_dist(2);
	right = get_dist(3);
}
int8_t RobotCar::ReturnSideOfObstacle()
{
	
}

int main()
{
	std::cout << "Hello World!\n";
}
