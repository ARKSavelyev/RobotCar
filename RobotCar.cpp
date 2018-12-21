#include "pch.h"
#include <iostream>
//фейковые функции, чтобы во время написания кода их вызовы не подсвечивались как ошибка
float get_dist(int nOfSonar)
{
	return 0;
}

void car_right_fi(int angle)
{

}

void Sleep(int timeMS)
{

}
//конец фейковых функций


class RobotCar
{
public:
	float x;
	float y;
	RobotCar();
	RobotCar(int newSpeed);
	//int ReturnSideOfObstacle();
	void Drive();
	void Stop();
	void TurnRight(int angle);//turn the car (I assume that giving negative angle will result in the car turning left)
	void UpdatePosition(int timeMS);
	float GetCenter(); // get distance to the obstacle from central sensor, you get the gist for next two methods
	float GetLeft();
	float GetRight();
	int GetAngle();
private:
	int _speed;
	bool _isMoving;
	float _right;
	float _center;
	float _left;
	int _currentAngle; //stores the current angle of the car from twelve o'clock (if looking left, it's 270, back 180)
	void _UpdateSensors();// get the distances from each sensor
};
RobotCar::RobotCar()
{
	_isMoving = false;
	_currentAngle = 0;
	x = 0;
	y = 0;
	_speed = defaultSpeed;
}
RobotCar::RobotCar(int newSpeed)
{
	_isMoving = false;
	_currentAngle = 0;
	x = 0;
	y = 0;
	_speed = newSpeed;
}
void RobotCar::UpdatePosition(int timeMS)
{
	float rad = Deg2Rad(_currentAngle);
	if (_isMoving)
	{
		x += cos(rad) * (_speed*(timeMS / 1000));
		x += sin(rad) * (_speed*(timeMS / 1000));
	}
}
void RobotCar::_UpdateSensors() // get the distances from each sensor
{
	_left = get_dist(1);
	_center = get_dist(2);
	_right = get_dist(3);
}
void RobotCar::Drive()//method for starting the movement
{
	_isMoving = true;
	//Insert method for moving forward here;
}
void RobotCar::Stop()
{
	_isMoving = false;
	//Insert method for stopping the car
}
void RobotCar::TurnRight(int angle)//turn the car (I assume that giving negative angle will result in the car turning left)
{
	/* Ignore the commented bits, not needed now
	//all these ifs are basically to make sure _currentAngle is within (0,360) range 
	if (angle < 0)
	{
		if (_currentAngle < abs(angle))
		{
			_currentAngle = 360 - abs(_currentAngle + angle);
		}
		else
		{
			_currentAngle = abs(_currentAngle + angle);
		}
	}
	else
	{
		if ((_currentAngle + angle) > 360)
		{
			_currentAngle = (_currentAngle + angle) - 360;
		}
		else
		{
			_currentAngle = _currentAngle + angle;
		}
	}
	*/
	car_right_fi(angle-_currentAngle);
	_currentAngle = angle;
}
int RobotCar::GetAngle()
{
	return _currentAngle;
}
float RobotCar::GetCenter()
{
	//_UpdateSensors;
	//return _center;
	return get_dist(2);
}
float RobotCar::GetLeft()
{
	//_UpdateSensors;
	//return _left;
	return get_dist(1);
}
float RobotCar::GetRight()
{
	//_UpdateSensors;
	//return _right;
	return get_dist(3);
}

/*int RobotCar::ReturnSideOfObstacle()
{

}*/


//NON CLASS Functions

//CHeck if car has arrived;
bool hasArrived(float carX, float carY, float goalX, float goalY)
{
	if ((abs(goalX - carX) < goalMargin) & (abs(goalY - carY) < goalMargin))
		return true;
	return false;
}

int Rad2Deg(float rad) //Degrees to radians
{
	return ((rad * 180) / pi);
}

float Deg2Rad(int deg) //Degrees to radians
{
	return (deg * pi) / 180;
}

float cal_angle(float current_x, float current_y, float tar_x, float tar_y) //calculate the angle, still a bit confused which angle I get (in relation to x or y), need to modify a bit so that angle is from 12 o'clock as well
{
	return Rad2Deg(atan2(tar_y - current_y, tar_x - current_x)); //it returns it in radians, need to change to degrees
}

//Extra variables needed for algorithm
//Currently here but when I get the working version will move to RobotCar class if deem it necessary
const int defaultSpeed = 5;
const float pi = 3.14159;

int timeMs = 100; //time which to send to sleep command, also the same time will be sent to the robotCar to update X and Y
float goalMargin = 0.1; //error margin for distance check (car does not have to arrive at precise point, within a radius would be fine)
float obstMargin = 0.2; //error margin for when measuring distance to the obstacle
//Those are not the same and same values is me not paying attention to it. SHOULD be adjusted however needed.

int main()
{
	float goalX;
	float goalY;

	float startX;
	float startY;

	RobotCar RobotCar;
	
	while (!hasArrived(RobotCar.x, RobotCar.y, goalX, goalY)) //Cycle continues until car reaches the point, or else, what's the fucking point?
	{
		RobotCar.UpdatePosition(timeMs);
		int carAngle = RobotCar.GetAngle(); //get current car angle 
		int angle = cal_angle(RobotCar.x, RobotCar.y, goalX, goalY); //get angle from car to the point relative to the 12 o clock or y positive axis
		if (carAngle != angle) //if not facing in the direction of the goal
		{
			float rightSensor = RobotCar.GetRight;
			if (rightSensor < obstMargin) //if there is some shit to the right then in theory there is an obstacle in the way, so continue along
			{
				RobotCar.Drive();
			}
			else
			{
				RobotCar.TurnRight(angle);
				if (RobotCar.GetCenter < 1)
					RobotCar.TurnRight(-angle);
				RobotCar.Drive();
			}
		}
		else
		{
			float centerSensor = RobotCar.GetCenter();
			if (centerSensor > obstMargin)
			{
				RobotCar.Drive();
			}
			else
			{
				do //basically, turn right in small increments, until there are now obstacles in front at least for a meter (as you can see, not using the full extend of sensor range at the moment)
				{
					RobotCar.TurnRight(-5);//
					centerSensor = RobotCar.GetCenter();
				} while (centerSensor<1);
				RobotCar.Drive();
			}

		}//there is some code refactoring possible (I already see some ways), but i leave it as it is for better readability for now, once it works wil lstart to refactor.
		Sleep(timeMs);//Basically, I'll do all checks ten times a seconds, because why the fuck not.
	}
	RobotCar.Stop();

}
