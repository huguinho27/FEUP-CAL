#pragma once
#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;

class Destination
{
private:
	string morada;
	int id;
	double latitude;
	double longitude;
public:
	Destination();
	Destination(string morada, double latitude, double longitude, int id);
	string getMorada() const;
	double getLatitude() const;
	double getLongitude() const;
	int getID() const;
	bool operator==(const Destination &left);
	double getDistance(Destination v);
};

ostream& operator<<(ostream& os, const Destination& d);
