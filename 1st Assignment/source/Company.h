#pragma once

#include "Item.h"
#include "Destination.h"
#include "Street.h"
#include "Graph.h"
#include "Truck.h"
#include "graphviewer.h"
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

	Graph<Destination> destinos;
	vector<Truck> fleet;
	vector<Item> itens;
public:
	Company();
	bool setGarage(int idNew);
	bool setDepot(int idNew);
	void SaveGarageDepot();
	void LoadGarageDepot();
	void LoadItens();
	void LoadCamioes();
	void defineGarageDepot();
	void SaveCamioes();
	void SaveItens();
	void ReadMapInfo();
	void ShowTrucks();
	void CreateNewTruck();
	void DeleteTruck();
	void ShowItems();
	void CreateNewItem();
	void DeleteItem();
	void ShowMap(Graph<Destination> map);
	void ShowDeliveryPoints();
	void ShowRoute(Solution<Destination> sol);
	double CalcDistance(double lat1, double long1, double lat2, double long2);
	void AssignItemsToTrucks();
	void CalcRoutes(bool bruteForce);
	void CalcRoutesBruteForce();
	void CalcRoutesGreedy();
	void MainMenu();
	Graph<Destination> getDestinos() const { return destinos; }
	vector<Truck> getCamioes() const { return fleet; }
	vector<Item> getItens() const { return itens; }
};
