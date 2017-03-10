#include "Destination.h"
#include <iostream>

using namespace std;

Destination::Destination(){}

Destination::Destination(string morada, double latitude, double longitude, int id)
{
	this->morada = morada;
	this->latitude = latitude;
	this->longitude = longitude;
	this->id = id;
}

string Destination::getMorada() const
{ return this->morada; }

double Destination::getLatitude() const
{ return this->latitude; }

double Destination::getLongitude() const
{ return this->longitude; }

int Destination::getID() const
{ return this->id; }

bool Destination::operator==(const Destination &left)
{
	return (getID() == left.getID());
}

double Destination::getDistance(Destination v)
{
	double delta_lat = abs(getLatitude() - v.getLatitude());
	double delta_lon = abs(getLongitude() - v.getLongitude());
	double r = 6371000; // raio da terra

	double a = sin(delta_lat / 2)*sin(delta_lat / 2) + cos(getLatitude())*cos(v.getLatitude())*sin(delta_lon / 2)*sin(delta_lon / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));

	return r * c;
}

ostream& operator<<(ostream& os, const Destination& d)
{
    os << d.getID();
    return os;
}

