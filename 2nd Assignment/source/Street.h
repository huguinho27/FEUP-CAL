#pragma once
#include <iostream>
using namespace std;

class Street
{
	unsigned long long id;
	string name;
	bool twoWays;
public:
	Street();
	Street(unsigned long long idAresta, string street, bool twoWays);
	unsigned long long getID() const;
	string getName() const;
	bool isTwoWays() const;
};
