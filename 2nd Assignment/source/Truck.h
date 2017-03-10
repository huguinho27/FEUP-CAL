#pragma once
#include <iostream>
#include <vector>
#include "Item.h"
#include "Graph.h"

using namespace std;

class Truck
{
protected:
	int id;
	double capacidade, current_cap = 0;
	vector<Item> itens;
	Graph<Destination> entregas;
	Solution<Destination> route;
public:
	Truck();
	Truck(int id, double capacidade);
	double getCapacidade() const;
	double getCurrentCap() {return current_cap;}
	int getID() const;
	bool addItem(Item i);
	bool operator<(const Truck& right) const;
	double getFreeWeight() const;
	vector<Item> getItems() { return itens;}
	void clearItens(){ itens.clear(); current_cap = 0; }
	Solution<Destination> getRoute(){return route;};
	void calcRoute(Destination start, Destination end, Graph<Destination> map, bool bruteforce);
};
