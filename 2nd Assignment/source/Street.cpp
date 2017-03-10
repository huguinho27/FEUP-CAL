#include "Street.h"
using namespace std;

Street::Street(){}

Street::Street(unsigned long long id, string name, bool twoWays)
{
	this->id = id;
	this->name = name;
	this->twoWays = twoWays;
}

unsigned long long Street::getID() const
{
	return id;
}

string Street::getName() const
{
	return name;
}

bool Street::isTwoWays() const
{
	return twoWays;
}
