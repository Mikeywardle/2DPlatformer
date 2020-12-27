#include "RayCasting.h"

Ray::Ray(Vector3 Start, Vector3 End)
{
	this->Start = Start;
	this->End = End;

	Direction = Vector3::Normalize(End - Start);
}
