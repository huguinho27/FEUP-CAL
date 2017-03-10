#include <stdlib.h>     /* atoi */
#include <math.h>
#include <unordered_map>
#include <string>

#include "Company.h"

using namespace std;

Company::Company(){}

double Company::CalcDistance(double lat1, double lon1, double lat2, double lon2)
{
	double R = 6371000;
	double lat1Rad = lat1*2*PI/360;
	double lat2Rad = lat2*2*PI/360;
	double deltaLat = (lat2-lat1)*2*PI/360;
	double deltaLong = (lon2-lon1)*2*PI/360;

	double a = sin(deltaLat/2) * sin(deltaLat/2) +
			cos(lat1Rad) * cos(lat2Rad) *
			sin(deltaLong /2) * sin(deltaLong /2);

	double c = 2 * atan2(sqrt(a), sqrt(1-a));

	return R * c;
}

void Company::LoadItems()
{
	items.clear();
	ifstream inFile("input/itens.txt");

	if (!inFile)
	{
		cerr << "N�o foi poss�vel abrir o ficheiro itens.txt";
		exit(1);
	}

	string line;
	int numfatura, id;
	double valor, peso;
	string nome;

	while (!inFile.eof())
	{
		getline(inFile, line);
		if(line == "") continue;
		stringstream linestream(line);
		string data;

		getline(linestream, data, ';');
		nome = data;
		linestream >> valor;
		getline(linestream, data, ';');
		linestream >> peso;
		getline(linestream, data, ';');
		linestream >> numfatura;
		getline(linestream, data, ';');
		linestream >> id;

		//cout << "Nome " << nome << " Valor " << valor << " Peso " << peso << " numero factura " << numfatura << " id destino" << id << endl;

		Item i(nome, valor, peso, numfatura, id);
		items.push_back(i);
	}
	inFile.close();
}

void Company::LoadTrucks()
{
	fleet.clear();
	ifstream inFile("input/camioes.txt");

	if (!inFile)
	{
		cerr << "N�o foi poss�vel abrir o ficheiro camioes.txt";
		exit(1);
	}

	string line;
	int id;
	double capacidade;

	while (!inFile.eof())
	{
		getline(inFile, line);
		if (line == "") continue;
		stringstream linestream(line);
		string data;

		linestream >> id;

		getline(linestream, data, ';');
		linestream >> capacidade;

		Truck c(id, capacidade);
		fleet.push_back(c);
	}
	inFile.close();
}

void Company::LoadGarageDepot()
{
	ifstream inFile("input/garagem_deposito.txt");

	if (!inFile) {
		cerr << "N�o foi poss�vel abrir o ficheiro garagem_deposito.txt";
		exit(1);
	}

	string line, data;
	getline(inFile, line);

	stringstream linestream(line);
	linestream >> garageID;

	getline(linestream, data, ';');
	linestream >> depotID;

	inFile.close();
}

void Company::SaveTrucks()
{
	ofstream of("input/camioes.txt");
	of.clear();

	for (unsigned int i = 0; i < fleet.size(); i++)
	{
		of << fleet[i].getID() << ";" << fleet[i].getCapacidade() << "\n";
	}
	of.close();
}

void Company::SaveItems()
{
	ofstream of("input/itens.txt");
	of.clear();

	for (unsigned int i = 0; i < items.size(); i++)
	{
		of << items[i].getRecipient() << ";" << items[i].getValue() << ";" <<
			items[i].getWeight() << ";" << items[i].getInvoiceNumber() << ";" << items[i].getDestinationID() << "\n";
	}
	of.close();
}

void Company::SaveGarageDepot()
{
	ofstream of("input/garagem_deposito.txt");
	of.clear();
	of << garageID << ";" << depotID;
	of.close();
}

void Company::ReadMapInfo()
{
	cout << "A importar informacoes do mapa..." << endl;

	/////////////////////////////IMAGEM//////////////////////////////////////////

	ifstream inFile("input/mapa_info.txt");

	if (!inFile) {
		cerr << "Nao foi poss�vel abrir o ficheiro mapa_info.txt";
		exit(1);
	}

	string line;

	getline(inFile, line);

	stringstream linestream(line);
	string data;

	linestream >> mapWidth;
	getline(linestream, data, ';');
	linestream >> mapHeight;
	getline(linestream, data, ';');
	linestream >> minLat;
	getline(linestream, data, ';');
	linestream >> minLong;
	getline(linestream, data, ';');
	linestream >> maxLat;
	getline(linestream, data, ';');
	linestream >> maxLong;

	//cout << "mapWidth " << mapWidth << " mapHeight " << mapHeight << " minLat " << minLat << " minLong " << minLong << " maxLat " << maxLat << " maxLong " << maxLong << endl << endl;

	inFile.close();

	/////////////////////////////NOS/////////////////////////////////////////////

	inFile.open("input/nos.txt");

	if (!inFile) {
		cerr << "Nao foi poss�vel abrir o ficheiro nodes.txt";
		exit(1);
	}

	unordered_map<unsigned long long, int> map;
	unsigned long long idNode = 0;
	int id = 0;
	double latitude = 0;
	double longitude = 0;

	while ( getline(inFile, line) )
	{
		stringstream linestream(line);
		string data;

		linestream >> idNode;
		getline(linestream, data, ';');
		linestream >> latitude;
		getline(linestream, data, ';');
		linestream >> longitude;

		map[idNode] = id;
		Destination d("", latitude, longitude, id);
		destinations.addVertex(d);
		id++;
	}

	inFile.close();

	/////////////////////////////RUAS/////////////////////////////////////////////

	inFile.open("input/ruas.txt");

	if (!inFile) {
		cerr << "Nao foi poss�vel abrir o ficheiro ruas.txt";
		exit(1);
	}

	vector<Street> streets;

	unsigned long long idStreet = 0;
	string roadName = "";
	bool twoWays;

	while (getline(inFile, line)) {
		stringstream linestream(line);
		string data;

		linestream >> idStreet;

		getline(linestream, data, ';');
		getline(linestream, data, ';');
		roadName = data;

		getline(linestream, data, ';');
		if (data == "False") twoWays = false;
		else twoWays = true;

		Street a(idStreet, roadName, twoWays);
		//cout << "street id: " << a.getID() << " roadName: " << a.getName() << " twoWays: " << a.isTwoWays() << endl;
		streets.push_back(a);
	}

	inFile.close();

	/////////////////////////////ARESTAS/////////////////////////////////////////////

	inFile.open("input/arestas.txt");

	if (!inFile) {
		cerr << "Nao foi poss�vel abrir o ficheiro streets.txt";
		exit(1);
	}

	unsigned long long idNodeStart = 0;
	unsigned long long idNodeEnd = 0;

	while (getline(inFile, line))
	{
		stringstream linestream(line);
		string data;

		linestream >> idStreet;

		getline(linestream, data, ';');
		linestream >> idNodeStart;

		getline(linestream, data, ';');
		linestream >> idNodeEnd;

		unsigned int i = 0;
		for (i = 0; i < streets.size(); i++)
		{
			if (streets[i].getID() == idStreet)
				break;
		}
		//cout << "idStreet: " << idStreet << " idNodeStart: " << idNodeStart << " idNodeEnd: " << idNodeEnd << " i: " << i << endl;

		Destination nodeStart("", 0, 0, map[idNodeStart]);
		Vertex<Destination> * vNodeStart = destinations.getVertex(nodeStart);
		nodeStart = vNodeStart->getInfo();
		if (nodeStart.getAddress() == ""){
			//cout << "Rua: " << streets[i].getName() << endl;
			nodeStart.setAddress(streets[i].getName());
			vNodeStart->setInfo(nodeStart);
		}

		Destination nodeEnd("", 0, 0, map[idNodeEnd]);
		Vertex<Destination> * vNodeEnd = destinations.getVertex(nodeEnd);
		nodeEnd = vNodeEnd->getInfo();
		if (nodeEnd.getAddress() == ""){
			nodeEnd.setAddress(streets[i].getName());
			vNodeEnd->setInfo(nodeEnd);
		}

		double dist = CalcDistance(nodeStart.getLatitude(), nodeStart.getLongitude(), nodeEnd.getLatitude(), nodeEnd.getLongitude());
		//cout << "idStreet: " << idStreet << " nodeStart: " << nodeStart.getID() << " nodeEnd: " << nodeEnd.getID() << " dist: " << dist << endl;
		destinations.addEdge(nodeStart, nodeEnd, dist);

		if (streets[i].isTwoWays() )
			destinations.addEdge(nodeEnd, nodeStart, dist);
	}

	inFile.close();

	cout << "Terminado!" << endl << endl;
}

void Company::ShowMap(Graph<Destination> map)
{
	GraphViewer *gv = new GraphViewer(mapWidth, mapHeight, false);

	gv->setBackground(mapImg);
	gv->createWindow(1200, 900);
	gv->defineEdgeDashed(true);

	vector<Vertex<Destination> *> points = map.getVertexSet();

	int ay = mapHeight/(minLat - maxLat);
	int by = mapHeight - ay*minLat;
	int ax = mapWidth/(maxLong - minLong);
	int bx = mapWidth - ax*maxLong;

	for (unsigned int i = 0; i < points.size(); i++)
	{
		Vertex<Destination> * v = points[i];
		int id = v->getInfo().getID();
		double lat = v->getInfo().getLatitude();
		double longitude = v->getInfo().getLongitude();
		int y = ay*lat+by;
		int x = ax*longitude+bx;
		gv->addNode(id, x, y);
	}

	int edgeId = 0;
	for (unsigned int i = 0; i < points.size(); i++)
	{
		Vertex<Destination> * v = points[i];
		int idSource = v->getInfo().getID();
		vector<Edge<Destination> > adj = v->getEdges();
		for (unsigned int j = 0; j < adj.size(); j++)
		{
			int idDestination = adj[j].getDestination()->getInfo().getID();
			gv->addEdge(edgeId, idSource, idDestination, EdgeType::DIRECTED);
			edgeId++;
		}
	}

	if (garageID != -1){
		gv->setVertexColor(garageID, "green");
		gv->setVertexLabel(garageID, "GARAGEM");
		gv->setVertexSize(garageID, 40);
	}

	if (depotID != -1){
		gv->setVertexColor(depotID, "blue");
		gv->setVertexLabel(depotID, "DEPOSITO");
		gv->setVertexSize(depotID, 40);
	}
}

void Company::CreateNewTruck()
{
	int id;
	double capacity;

	system("cls");
	cout << "--CRIAR NOVO CAMIAO--" << endl << endl;
	cout << "QUAL O ID DO CAMIAO A CRIAR?" << endl;
	cin >> id;
	if(!cin)
	{
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "NUMERO INVALIDO (PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
		return;
	}
	else if(id == 0){
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "ID NAO PODE SER ZERO (PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
		return;
	}
	cin.clear();
	cin.ignore(10000, '\n');

	for (unsigned int i = 0; i < fleet.size(); i++)
	{
		if( fleet[i].getID() == id)
		{
			cout << "JA EXISTE UM CAMIAO COM ESSE ID (PRESSIONE ENTER PARA VOLTAR)" << endl;
			cin.get();
			return;
		}
	}

	cout << "QUAL A CAPACIDADE DO CAMIAO A CRIAR?" << endl;
	cin >> capacity;
	if(!cin)
	{
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "NUMERO INVALIDO (PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
		return;
	}
	cin.clear();
	cin.ignore(10000, '\n');

	Truck c(id, capacity);
	fleet.push_back(c);

	cout << "CAMIAO CRIADO COM SUCESSO (PRESSIONE ENTER PARA VOLTAR)" << endl;
	cin.get();
}

void Company::DeleteTruck()
{
	int id;
	system("cls");
	cout << "--ELIMINAR CAMIAO--" << endl << endl;
	cout << "QUAL O ID DO CAMIAO A ELIMINAR?" << endl;

	cin >> id;
	if(!cin)
	{
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "NUMERO INVALIDO (PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
		return;
	}
	cin.clear();
	cin.ignore(10000, '\n');

	vector<Truck>::iterator it;
	for (it = fleet.begin(); it != fleet.end(); it++)
	{
		if( it->getID() == id)
		{
			fleet.erase(it);
			cout << "CAMIAO " << id << " ELIMINADO COM SUCESSO (PRESSIONE ENTER PARA VOLTAR)" << endl;
			cin.get();
			return;
		}
	}
	cout << "NAO EXISTE NENHUM CAMIAO " << id << " (PRESSIONE ENTER PARA VOLTAR)" << endl;
	cin.get();
	return;
}

void Company::ShowTrucks()
{
	int opt;
	system("cls");
	if (fleet.size() != 0)
	{
		cout << "---------------------------------------------------" << endl;
		for (unsigned int i = 0; i < fleet.size(); i++) {
			cout << "CAMIAO " << fleet[i].getID() << endl;


			cout << "Capacidade: " << fleet[i].getCapacidade() << endl << endl;
		}
		cout << "---------------------------------------------------" << endl;
	}
	else
		cout << "NAO HA CAMIOES PARA MOSTRAR" << "\n";

	do {
		cout << "1 - CRIAR NOVO CAMIAO" << "\n";
		cout << "2 - ELIMINAR CAMIAO" << "\n";
		cout << "3 - VOLTAR AO MENU" << "\n";
		cin >> opt;
		if(!cin)
			opt = -1;
		cin.clear();
		cin.ignore(10000, '\n');

	} while (opt < 1 || opt > 3);

	if (opt == 1)
		CreateNewTruck();
	else if (opt == 2)
		DeleteTruck();
}

void Company::CreateNewItem()
{
	string recipientName;
	double value;
	double weight;
	int invoiceNumber, id;

	system("cls");
	cout << "--CRIAR NOVO ITEM--" << endl << endl;
	cout << "QUAL O NUMERO DE FATURA DO ITEM?" << endl;
	cin >> invoiceNumber;
	if(!cin)
	{
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "NUMERO INVALIDO (PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
		return;
	}
	cin.clear();
	cin.ignore(10000, '\n');

	for (unsigned int i = 0; i < items.size(); i++)
	{
		if( items[i].getInvoiceNumber() == invoiceNumber)
		{
			cout << "JA EXISTE UM ITEM COM ESSE NUMERO DE FACTURA (PRESSIONE ENTER PARA VOLTAR)" << endl;
			cin.get();
			return;
		}
	}

	cout << "QUAL O ID DO DESTINO?" << endl;
	cin.clear();
	cin >> id;
	cin.ignore(10000, '\n');
	Destination d = Destination("", 0, 0, id);
	Vertex<Destination> * v = destinations.getVertex(d);
	if (v == NULL)
	{
		cout << "NAO EXISTE NENHUM DESTINO COM ESSE ID (PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
		return;
	}

	cout << "QUAL O NOME DO DESTINATARIO?" << endl;
	getline(cin, recipientName);
	//cin.clear();
	//cin.ignore(10000, '\n');

	cout << "QUAL O VALOR DO ITEM?" << endl;
	cin >> value;
	if(!cin)
	{
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "VALOR INVALIDO (PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
		return;
	}
	cin.clear();
	cin.ignore(10000, '\n');

	cout << "QUAL O PESO DO ITEM?" << endl;
	cin >> weight;
	if(!cin)
	{
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "PESO INVALIDO (PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
		return;
	}
	cin.clear();
	cin.ignore(10000, '\n');

	Item i(recipientName, value, weight, invoiceNumber, id);
	items.push_back(i);

	cout << "ITEM CRIADO COM SUCESSO (PRESSIONE ENTER PARA VOLTAR)" << endl;
	cin.get();
}

void Company::DeleteItem()
{
	int n;
	system("cls");
	cout << "--ELIMINAR ITEM--" << endl << endl;
	cout << "QUAL O NUMERO DE FACTURA DO ITEM A ELIMINAR?" << endl;
	cin >> n;
	if(!cin)
		n = -1;
	cin.clear();
	cin.ignore(10000, '\n');

	vector<Item>::iterator it;
	for (it = items.begin(); it != items.end(); it++)
	{
		if( it->getInvoiceNumber() == n)
		{
			items.erase(it);
			cout << "ITEM COM FACTURA " << n << " ELIMINADO COM SUCESSO (PRESSIONE ENTER PARA VOLTAR)" << endl;
			cin.get();
			return;
		}
	}
	cout << "NAO EXISTE NENHUM ITEM COM FACTURA " << n << " (PRESSIONE ENTER PARA VOLTAR)" << endl;
	cin.get();
	return;
}

void Company::ShowItems()
{
	int opt;
	system("cls");
	if (items.size() != 0)
	{
		for (unsigned int i = 0; i < items.size(); i++)
		{
			Destination d("", 0, 0, items[i].getDestinationID() );
			d = destinations.getVertex(d)->getInfo();

			cout << "---------------------------------------------------" << endl;
			cout << "Item com fatura numero " << items[i].getInvoiceNumber() << endl;
			cout << "ID do destino: " << items[i].getDestinationID() << endl;
			cout << "Destinatario: " << items[i].getRecipient() << endl;
			cout << "Morada: " << d.getAddress() << endl;
			cout << "Peso: " << items[i].getWeight() << endl;
			cout << "Valor: " << items[i].getValue() << endl;
		}
		cout << "---------------------------------------------------"<< endl;
	}
	else{
		cout << "---------------------------------------------------"<< endl;
		cout << "NAO HA ITENS PARA MOSTRAR" << endl;
		cout << "---------------------------------------------------"<< endl;
	}

	do {
		cout << "1 - CRIAR NOVO ITEM" << endl;
		cout << "2 - ELIMINAR ITEM" << endl;
		cout << "3 - VOLTAR AO MENU" << endl;
		cin >> opt;
		if(!cin)
			opt = -1;
		cin.clear();
		cin.ignore(10000, '\n');
	} while (opt < 1 || opt > 3);

	if (opt == 1)
		CreateNewItem();
	else if (opt == 2)
		DeleteItem();
}

void Company::ShowDeliveryPoints(vector<Item> items)
{
	GraphViewer *gv = new GraphViewer(mapWidth, mapHeight, false);

	gv->setBackground(mapImg);
	gv->createWindow(1200, 900);
	gv->defineEdgeDashed(true);

	int ay = mapHeight/(minLat - maxLat);
	int by = mapHeight - ay*minLat;
	int ax = mapWidth/(maxLong - minLong);
	int bx = mapWidth - ax*maxLong;

	for (unsigned int i = 0; i < items.size(); i++)
	{
		Destination d = Destination("", 0, 0, items[i].getDestinationID() );
		Vertex<Destination> * v = destinations.getVertex(d);
		int id = v->getInfo().getID();
		double lat = v->getInfo().getLatitude();
		double longitude = v->getInfo().getLongitude();
		int y = ay*lat+by;
		int x = ax*longitude+bx;
		gv->addNode(id, x, y);

		std::ostringstream stm ;
		stm << "ITEM #"<< items[i].getInvoiceNumber();
		gv->setVertexLabel(id, stm.str());
		gv->setVertexColor(id, "red");
		gv->setVertexSize(id, 25);
	}
}

void Company::ShowRoute(Solution<Destination> sol)
{
	GraphViewer *gv = new GraphViewer(mapWidth, mapHeight, false);

	gv->setBackground(mapImg);
	gv->createWindow(1200, 900);
	gv->defineEdgeDashed(true);

	vector<Vertex<Destination> *> points = destinations.getVertexSet();

	int ay = mapHeight/(minLat - maxLat);
	int by = mapHeight - ay*minLat;
	int ax = mapWidth/(maxLong - minLong);
	int bx = mapWidth - ax*maxLong;

	int edgeId = 0;
	int previousID = 0;
	for (unsigned int i = 0; i < sol.path.size(); i++)
	{
		Vertex<Destination> * v = sol.path[i];
		int id = v->getInfo().getID();
		double lat = v->getInfo().getLatitude();
		double longitude = v->getInfo().getLongitude();
		int y = ay*lat+by;
		int x = ax*longitude+bx;
		gv->addNode(id, x, y);
		if(i != 0){
			gv->addEdge(edgeId, previousID, id, EdgeType::DIRECTED);
			edgeId++;
		}
		previousID = id;
	}

	if (garageID != -1){
		gv->setVertexColor(garageID, "green");
		gv->setVertexLabel(garageID, "GARAGEM");
		gv->setVertexSize(garageID, 40);
	}

	if (depotID != -1){
		gv->setVertexColor(depotID, "blue");
		gv->setVertexLabel(depotID, "DEPOSITO");
		gv->setVertexSize(depotID, 40);
	}
}

bool cmpItems(Item i, Item j) { return (j<i); }

bool sortTrucksCapacity(Truck i, Truck j) { return (j<i); }

bool sortTrucksId(Truck i, Truck j) { return (i.getID()<j.getID()); }

void Company::AssignItemsToTrucks()
{
	system("cls");
	if (garageID == -1 || depotID == -1)
	{
		cout << "GARAGEM E/OU DEPOSITO NAO ESTAO DEFINIDOS" << endl << "(PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
		return;
	}
	
	// vector ordenado por ordem decrescente
	vector<Item> items1 = items;
	sort(items1.begin(), items1.end(), cmpItems);

	vector<Item> itemsToBig;
	vector<Item> itemsNotLinked;

	if (fleet.size() == 0)
	{
		cout << "A FROTA NAO POSSUI CAMIOES" << endl << "(PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
		return;
	}

	for(unsigned int i=0; i<fleet.size(); i++)
		fleet[i].clearItens();
	sort(fleet.begin(), fleet.end(), sortTrucksCapacity);

	vector<Destination> poi;

	poi.push_back( Destination("", 0, 0, garageID) );
	poi.push_back( Destination("", 0, 0, depotID) );

	for(unsigned int i=0; i<items1.size(); i++)
	{
		poi.push_back( Destination("", 0, 0, items1[i].getDestinationID()) );
	}
	Graph<Destination> deliveries = destinations.makeGraphWithPOI(poi);

	//Mostra grafo de trabalho
	//ShowMap(deliveries);

	//Verifica se todos os pontos sao acessiveis da garagem, caso contrario, remove do grafo
	Vertex<Destination> * v = deliveries.getVertex( Destination("", 0, 0, garageID) );
	vector<Edge<Destination> > ed = v->getEdges();

	//Verifica deposito
	bool linked = false;
	for(unsigned int i=0; i< ed.size(); i++)
	{
		if(ed[i].getDestination()->getInfo().getID() == depotID){
			linked = true;
			break;
		}
	}
	//Se nao tiver ligacao ao deposito interrompe algoritmo
	if (!linked)
	{
		cout << "O DEPOSITO NAO ESTA ACESSIVEL DA GARAGEM - DEFINA NOVOS LOCAIS " << endl << "(PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
		return;
	}

	//Verifica ligacao de todos os itens
	vector<Item>::iterator it = items1.begin();
	while(it != items1.end())
	{
		linked = false;
		for(unsigned int j = 0; j < ed.size(); j++)
		{
			if(it->getDestinationID() == ed[j].getDestination()->getInfo().getID())
			{
				vector<Edge<Destination> > ed2 = ed[j].getDestination()->getEdges();
				for(unsigned int i = 0; i < ed2.size(); i++)
				{
					if(ed2[i].getDestination()->getInfo().getID() == garageID)
					{
						linked = true;
						break;
					}
				}
				break;
			}
		}
		//Se nao tiver ligacao remove do grafo e do vector de itens temporario
		if(!linked){
			deliveries.removeVertex( Destination("", 0, 0, it->getDestinationID()) );
			itemsNotLinked.push_back( *it );
			items1.erase(it);
		} else it++;
	}
	//Remove garagem e deposito do grafo pois ja nao sao precisos
	deliveries.removeVertex( Destination("", 0, 0, garageID) );
	deliveries.removeVertex( Destination("", 0, 0, depotID) );

	// Come�a pelo item maior e tenta coloca-lo no camiao
	// Insere o item mais proximo do ultimo item e que caiba ate nao conseguir colocar mais nenhum item
	for(unsigned int i=0; i<fleet.size(); i++)
	{
		if(items1.size() == 0) break;

		while(fleet[i].getCapacidade() < items1.begin()->getWeight() && items1.size() > 0)
		{
			itemsToBig.push_back( *(items1.begin()) );
			items1.erase(items1.begin());
		}
		if(items1.size() == 0) break;

		fleet[i].addItem( *(items1.begin()) );
		items1.erase(items1.begin());
		bool truck_full = false;

		int n=0;
		while(!truck_full && items1.size() != 0)
		{
			Vertex<Destination> * v = deliveries.getVertex( Destination("", 0, 0, fleet[i].getItems().at(n).getDestinationID()) );
			vector<Edge<Destination> > edges = v->getEdges();
			if (edges.size() == 0)
			{
				//cout << "Erro: Ponto isolado" << endl;
				break;
			}
			sort( edges.begin(), edges.end() );

			unsigned int j=0;
			while( j < edges.size() )
			{
				vector<Item>::iterator it1 = items1.begin();
				for(it1 = items1.begin(); it1 != items1.end(); it1++)
				{
					if(it1->getDestinationID() == edges[j].getDestination()->getInfo().getID())
						break;
				}
				if(it1 == items1.end()){
					j++;
					continue;
				}
				if(it1->getWeight() + fleet[i].getCurrentCap() <= fleet[i].getCapacidade())
				{
					fleet[i].addItem(*it1);
					items1.erase(it1);
					break;
				}
				else if(j == edges.size()-1){
					truck_full = true;	// se mais nenhum item cabe, camiao esta cheio
					break;
				}
				j++;
			}
			n++;
		}

		cout << "---------------------------------------------------" << endl;
		cout << "CAMIAO " << fleet[i].getID() << " (Capacidade: " << fleet[i].getCurrentCap() << "/" << fleet[i].getCapacidade() << ")"<< endl << endl;
		vector<Item> itens = fleet[i].getItems();
		for(unsigned int i = 0; i < itens.size(); i++)
		{
			cout << "Item #" << itens[i].getInvoiceNumber()
					<< " (Peso: " << itens[i].getWeight()
					<< "ID Dest: " << itens[i].getDestinationID()
					<< ", Destinatario: " << itens[i].getRecipient()
					<< ", Valor: " << itens[i].getValue() << ")" << endl;
		}
		cout << endl;
	}
	cout << "---------------------------------------------------" << endl;
	sort(fleet.begin(), fleet.end(), sortTrucksId);

	while( itemsNotLinked.size() > 0)
	{
		cout << "ITEM COM FACTURA " << itemsNotLinked[0].getInvoiceNumber() << " NAO PODE SER ENTREGUE PORQUE ESTA INACESSIVEL" << endl;
		itemsNotLinked.erase(itemsNotLinked.begin());
	}

	while( itemsToBig.size() > 0)
	{
		cout << "ITEM COM FACTURA " << itemsToBig[0].getInvoiceNumber() << " NAO PODE SER TRANSPORTADO POR SER DEMASIADO GRANDE" << endl;
		itemsToBig.erase(itemsToBig.begin());
	}

	while( items1.size() > 0)
	{
		cout << "ITEM COM FACTURA " << items1[0].getInvoiceNumber() << " NAO PODE SER TRANSPORTADO POR FALTA DE CAPACIDADE DA FROTA" << endl;
		items1.erase(items1.begin());
	}

	cout << endl << "(PRESSIONE ENTER PARA VOLTAR)" << endl;
	cin.get();
	return;
}

void Company::CalcRoutes(bool bruteForce)
{
	if (garageID == -1 || depotID == -1)
	{
		cout << "GARAGEM E/OU DEPOSITO NAO ESTAO DEFINIDOS" << endl << "(PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
		return;
	}

	Destination g("", 0, 0, garageID);
	Destination d("", 0, 0, depotID);

	for(unsigned int i = 0; i < fleet.size(); i++)
	{
		if(fleet[i].getItems().size() > 0)
		{
			fleet[i].calcRoute(g, d, destinations, bruteForce);
			Solution<Destination> sol = fleet[i].getRoute();
			ShowRoute(sol);

			cout << "---------------------------------------------------" << endl;
			cout << "CAMIAO " << fleet[i].getID() << " (Capacidade: " << fleet[i].getCurrentCap() << "/" << fleet[i].getCapacidade() << ")"<< endl << endl;

			cout << "CAMINHO:" << endl;
			for(unsigned int i = 0; i < sol.path.size(); i++)
			{
				cout << "Destino #" << sol.path[i]->getInfo() << endl;
			}
			cout << endl;
			cout << "Total: " << sol.pathLength << endl;
		}
	}
	cout << "---------------------------------------------------" << endl;
	cout << "(PRESSIONE ENTER PARA VOLTAR)" << endl;
	cin.get();
}

void Company::CalcRoutesBruteForce()
{
	CalcRoutes(true);
}

void Company::CalcRoutesGreedy()
{
	CalcRoutes(false);
}

bool Company::SetGarage(int idNew)
{
	if ( NULL == destinations.getVertex(Destination("",0,0,idNew)) )
		return false;
	garageID = idNew;
	return true;
}

bool Company::SetDepot(int idNew)
{
	if ( NULL == destinations.getVertex(Destination("",0,0,idNew)) )
		return false;
	depotID = idNew;
	return true;
}

void Company::DefineGarageDepot()
{
	int id;
	system("cls");
	cout << "INDIQUE O ID DO DESTINO COMO GARAGEM" << "\n";
	cin >> id;
	if(!cin)
		id = -1;
	cin.clear();
	cin.ignore(10000, '\n');

	if( !SetGarage(id) )
	{
		cout << "ID DO DESTINO INVALIDO (PRESSIONE ENTER PARA VOLTAR)" << "\n";
		cin.get();
		return;
	}

	cout << "INDIQUE O ID DO DESTINO COMO DEPOSITO" << "\n";
	cin >> id;
	if(!cin)
		id = -1;
	cin.clear();
	cin.ignore(10000, '\n');

	if( !SetDepot(id) )
	{
		cout << "ID DO DESTINO INVALIDO (PRESSIONE ENTER PARA VOLTAR)" << "\n";
		cin.get();
		return;
	}

	cout << "GARAGEM E DEPOSITO DEFINIDOS COM SUCESSO (PRESSIONE ENTER PARA VOLTAR)" << "\n";
	cin.get();
}

void Company::SearchItems()
{
	int opt = -1;

	do {
		system("cls");
		cout << "PESQUISA DE ITENS" << endl;
		cout << "\n";
		cout << "1 - PESQUISA POR NOME PARCIAL DO CLIENTE" << endl;
		cout << "2 - PESQUISA POR NOME PARCIAL DO CLIENTE (MAIS RAPIDO)" << endl;
		cout << "3 - PESQUISA POR NOME COMPLETO DO CLIENTE (NOME APROXIMADO)" << endl;
		cout << "4 - PESQUISA POR ENDERECO PARCIAL" << endl;
		cout << "5 - PESQUISA POR ENDERECO PARCIAL (MAIS RAPIDO)" << endl;
		cout << "6 - PESQUISA POR ENDERECO COMPLETO (ENDERECO APROXIMADO)" << endl;
		cout << "0 - VOLTAR AO MENU PRINCIPAL" << endl;

		cin >> opt;
		if(!cin){
			opt = -1;
		}
		cin.clear();
		cin.ignore(10000, '\n');
	} while (opt < 0 || opt > 6);

	//CAMIAO
	string pattern, text;
	vector<Item> itemsToSearch;
	vector<Item> itemsWithMatch;

	system("cls");
	cout << "PESQUISA DE ITENS" << endl << endl;
	cout << "INSIRA O ID DO CAMIAO ONDE EFECTUAR PESQUISA (0 PARA PESQUISAR NO DEPOSITO)" << endl;

	int id;
	cin >> id;

	if(!cin){
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "NUMERO INVALIDO (PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
		return;
	}
	else if(id == 0){
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "SELECIONADA PESQUISA NO DEPOSITO" << endl;
		itemsToSearch = items;
	}
	else{
		cin.clear();
		cin.ignore(10000, '\n');

		bool found = false;
		vector<Truck>::iterator it;
		for (it = fleet.begin(); it != fleet.end(); it++)
		{
			if( it->getID() == id)
			{
				itemsToSearch = it->getItems();
				found = true;
				break;
			}
		}
		if (!found){
			cout << "NAO EXISTE NENHUM CAMIAO " << id << " (PRESSIONE ENTER PARA VOLTAR)" << endl;
			cin.get();
			return;
		}
	}

	//STRINGS
	if(opt >= 1 && opt <=3){
		cout << "INTRODUZA O NOME DO CLIENTE A PESQUISAR" << endl;
		getline(cin, pattern);
		int size = itemsToSearch.size();
		for (int i = 0; i < size; i++){
			text = itemsToSearch[i].getRecipient();
			if (opt == 1){
				if (findStringNaive(pattern, text)) itemsWithMatch.push_back(itemsToSearch[i]);
			}else if (opt == 2){
				if (KMP(pattern, text)) itemsWithMatch.push_back(itemsToSearch[i]);
			}else if (EditDistance(pattern, text) < 3) itemsWithMatch.push_back(itemsToSearch[i]);
		}
	}
	else{
		cout << "INTRODUZA O ENDERECO A PESQUISAR" << endl;
		getline(cin, pattern);
		int size = itemsToSearch.size();
		for (int i = 0; i < size; i++){
			Destination d("", 0, 0, itemsToSearch[i].getDestinationID() );
			text = destinations.getVertex(d)->getInfo().getAddress();
			if (opt == 4){
				if (findStringNaive(pattern, text)) itemsWithMatch.push_back(itemsToSearch[i]);
			}else if (opt == 5){
				if (KMP(pattern, text)) itemsWithMatch.push_back(itemsToSearch[i]);
			}else if (EditDistance(pattern, text) < 3) itemsWithMatch.push_back(itemsToSearch[i]);
		}
	}

	//RESULTADOS
	system("cls");
	if (itemsWithMatch.size() != 0)
	{
		cout << "---------------------------------------------------" << endl;
		cout << "ITENS QUE CORRESPONDEM A PESQUISA:" << endl;

		for (unsigned int i = 0; i < itemsWithMatch.size(); i++)
		{
			Destination d("", 0, 0, itemsWithMatch[i].getDestinationID() );
			d = destinations.getVertex(d)->getInfo();

			cout << "---------------------------------------------------" << endl;
			cout << "Item com fatura numero " << itemsWithMatch[i].getInvoiceNumber() << endl;
			cout << "ID do destino: " << itemsWithMatch[i].getDestinationID() << endl;
			cout << "Destinatario: " << itemsWithMatch[i].getRecipient() << endl;
			cout << "Morada: " << d.getAddress() << endl;
			cout << "Peso: " << itemsWithMatch[i].getWeight() << endl;
			cout << "Valor: " << itemsWithMatch[i].getValue() << endl;
		}
		cout << "---------------------------------------------------" << endl;

		ShowDeliveryPoints(itemsWithMatch);

		cout << endl << "(PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
	}
	else{
		cout << "---------------------------------------------------" << endl;
		cout << "NAO HA ITENS PARA MOSTRAR" << endl;
		cout << "---------------------------------------------------" << endl;

		cout << endl << "(PRESSIONE ENTER PARA VOLTAR)" << endl;
		cin.get();
	}
}

void Company::MainMenu()
{
	bool run = true;
	while(run)
	{
		int opt = -1;
		do{
			system("cls");
			cout << "BEM-VINDO AO PROGRAMA DE GESTAO DE ENTREGAS NO PORTO" << endl;
			cout << "\n";
			cout << "1 - VER MAPA" << endl;
			cout << "2 - DEFINIR GARAGEM E DEPOSITO" << endl;
			cout << "3 - GERIR CAMIOES" << endl;
			cout << "4 - GERIR ITENS" << endl;
			cout << "5 - VER PONTOS DE ENTREGA" << endl;
			cout << "6 - DISTRIBUIR ITENS PELOS CAMIOES" << endl;
			cout << "7 - CALCULAR ROTAS DE ENTREGA (SOLUCAO OPTIMA)" << endl;
			cout << "8 - CALCULAR ROTAS DE ENTREGA (SOLUCAO RAPIDA)" << endl;
			cout << "9 - PESQUISAR ITENS" << "\n";
			cout << "0 - SAIR" << "\n";

			cin >> opt;
			if(!cin){
				opt = -1;
			}
			cin.clear();
			cin.ignore(10000, '\n');


		} while (opt < 0 || opt > 9);

		switch (opt) {
		case 1:
			ShowMap(destinations);
			break;
		case 2:
			DefineGarageDepot();
			break;
		case 3:
			ShowTrucks();
			break;
		case 4:
			ShowItems();
			break;
		case 5:
			ShowDeliveryPoints(items);
			break;
		case 6:
			AssignItemsToTrucks();
			break;
		case 7:
			CalcRoutesBruteForce();
			break;
		case 8:
			CalcRoutesGreedy();
			break;
		case 9:
			SearchItems();
			break;
		case 0:
			run = false;
			break;
		default:
			break;
		}
	}
}



