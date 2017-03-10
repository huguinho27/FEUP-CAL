#pragma once

#include "Item.h"
#include "Destination.h"
#include "Street.h"
#include "Graph.h"
#include "Truck.h"
#include "graphviewer.h"
#include "StringSearch.h"
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <ctime>
#include <Windows.h>
#include <ctgmath>
#include <math.h>
#include <functional>   // std::greater
#include <algorithm>    // std::sort

constexpr auto PI = 3.141592653589793238463;

class Company
{
protected:
	int garageID = -1;
	int depotID = -1;

	string mapImg = "input/mapa_img.png";
	int mapWidth, mapHeight;
	double minLat, minLong, maxLat, maxLong;

	Graph<Destination> destinations;
	vector<Truck> fleet;
	vector<Item> items;
public:
	Company();
	bool SetGarage(int idNew);
	bool SetDepot(int idNew);
	void SaveGarageDepot();
	void LoadGarageDepot();
	void LoadItems();
	void LoadTrucks();
	void DefineGarageDepot();
	void SaveTrucks();
	void SaveItems();
	void ReadMapInfo();
	void ShowTrucks();
	void CreateNewTruck();
	void DeleteTruck();
	void ShowItems();
	void CreateNewItem();
	void DeleteItem();
	void SearchItems();
	void ShowMap(Graph<Destination> map);
	void ShowDeliveryPoints(vector<Item> items);
	void ShowRoute(Solution<Destination> sol);
	double CalcDistance(double lat1, double long1, double lat2, double long2);
	void AssignItemsToTrucks();
	void CalcRoutes(bool bruteForce);
	void CalcRoutesBruteForce();
	void CalcRoutesGreedy();
	void MainMenu();
	Graph<Destination> getDestinations() const { return destinations; }
	vector<Truck> getFleet() const { return fleet; }
	vector<Item> getItems() const { return items; }
};
