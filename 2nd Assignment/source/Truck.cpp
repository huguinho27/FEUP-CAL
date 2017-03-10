#include "Truck.h"

using namespace std;

Truck::Truck() {};

Truck::Truck(int id, double capacidade)
{
	this->capacidade = capacidade;
	this->id = id;
}

double Truck::getCapacidade() const
{ return this->capacidade;}

int Truck::getID() const
{return this->id;}

bool Truck::addItem(Item i)
{
	if (capacidade > current_cap + i.getWeight())
	{
		current_cap += i.getWeight();
		itens.push_back(i);
		return true;
	}
	return false;
}

bool Truck::operator<(const Truck& right) const
{
	return (capacidade < right.getCapacidade());
}

double Truck::getFreeWeight() const
{
	double sum = 0;

	for (unsigned int i = 0; i < itens.size();i++)
	{
		sum += itens[i].getWeight();
	}

	return capacidade - sum;
}

void Truck::calcRoute(Destination start, Destination end, Graph<Destination> map, bool bruteForce)
{
	vector<Destination> poi;
	poi.push_back(start);
	poi.push_back(end);

	if (itens.size() > 0)
	{
		for(unsigned int i=0; i<itens.size(); i++)
		{
			poi.push_back( Destination("", 0, 0, itens[i].getDestinationID()));
		}
		Graph<Destination> entregas = map.makeGraphWithPOI(poi);

		if(bruteForce)
			route = entregas.getShortestDeliveryPathBruteForce(start, end);
		else
			route = entregas.getShortestDeliveryPathGreedy(start, end);
	}
}
