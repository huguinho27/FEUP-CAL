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

void Company::LoadItens()
{
	itens.clear();
	ifstream inFile("input/itens.txt");

	if (!inFile)
	{
		cerr << "Unable to open file itens.txt";
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
		itens.push_back(i);
	}
	inFile.close();
}

void Company::LoadCamioes()
{
	fleet.clear();
	ifstream inFile("input/camioes.txt");

	if (!inFile)
	{
		cerr << "Unable to open file camioes.txt";
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
		cerr << "Não foi possível abrir o ficheiro garagem_deposito.txt";
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

void Company::SaveCamioes()
{
	ofstream of("input/camioes.txt");
	of.clear();

	for (unsigned int i = 0; i < fleet.size(); i++)
	{
		of << fleet[i].getID() << ";" << fleet[i].getCapacidade() << "\n";
	}
	of.close();
}

void Company::SaveItens()
{
	ofstream of("input/itens.txt");
	of.clear();

	for (unsigned int i = 0; i < itens.size(); i++)
	{
		of << itens[i].getDestinatario() << ";" << itens[i].getValor() << ";" <<
			itens[i].getPeso() << ";" << itens[i].getNrFatura() << ";" << itens[i].getDestino() << "\n";
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
	cout << "A importar informações do mapa..." << endl;

	/////////////////////////////IMAGEM//////////////////////////////////////////

	ifstream inFile("input/mapa_info.txt");

	if (!inFile) {
		cerr << "Não foi possível abrir o ficheiro mapa_info.txt";
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
		cerr << "Não foi possível abrir o ficheiro nodes.txt";
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
		destinos.addVertex(d);
		id++;
	}

	inFile.close();

	/////////////////////////////RUAS/////////////////////////////////////////////

	inFile.open("input/ruas.txt");

	if (!inFile) {
		cerr << "Não foi possível abrir o ficheiro ruas.txt";
		exit(1);
	}

	vector<Street> ruas;

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
		ruas.push_back(a);
	}

	inFile.close();

	/////////////////////////////ARESTAS/////////////////////////////////////////////

	inFile.open("input/arestas.txt");

	if (!inFile) {
		cerr << "Não foi possível abrir o ficheiro ruas.txt";
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
		for (i = 0; i < ruas.size(); i++)
		{
			if (ruas[i].getID() == idStreet)
				break;
		}
		//cout << "idStreet: " << idStreet << " idNodeStart: " << idNodeStart << " idNodeEnd: " << idNodeEnd << " i: " << i << endl;

		Destination origem("", 0, 0, map[idNodeStart]);
		origem = destinos.getVertex(origem)->getInfo();

		Destination destino("", 0, 0, map[idNodeEnd]);
		destino = destinos.getVertex(destino)->getInfo();

		double dist = CalcDistance(origem.getLatitude(),origem.getLongitude(),destino.getLatitude(),destino.getLongitude());
		//cout << "idStreet: " << idStreet << " origem: " << origem.getID() << " destino: " << destino.getID() << " dist: " << dist << endl;
		destinos.addEdge(origem, destino, dist);

		if (ruas[i].isTwoWays() )
			destinos.addEdge(destino, origem, dist);
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

	vector<Vertex<Destination> *> pontos = map.getVertexSet();

	int ay = mapHeight/(minLat - maxLat);
	int by = mapHeight - ay*minLat;
	int ax = mapWidth/(maxLong - minLong);
	int bx = mapWidth - ax*maxLong;

	for (unsigned int i = 0; i < pontos.size(); i++)
	{
		Vertex<Destination> * v = pontos[i];
		int id = v->getInfo().getID();
		double lat = v->getInfo().getLatitude();
		double longitude = v->getInfo().getLongitude();
		int y = ay*lat+by;
		int x = ax*longitude+bx;
		gv->addNode(id, x, y);
	}

	int edgeId = 0;
	for (unsigned int i = 0; i < pontos.size(); i++)
	{
		Vertex<Destination> * v = pontos[i];
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
	double capacidade;

	system("cls");
	cout << "--CRIAR NOVO CAMIAO--" << endl << endl;
	cout << "QUAL O ID DO CAMIAO A CRIAR?" << endl;
	cin.clear();
	cin >> id;

	for (unsigned int i = 0; i < fleet.size(); i++)
	{
		if( fleet[i].getID() == id)
		{
			cout << "JA EXISTE UM CAMIAO COM ESSE ID (PRESSIONE QUALQUER TECLA PARA VOLTAR)" << endl;
			cin.ignore();
			getchar();
			return;
		}
	}

	cout << "QUAL A CAPACIDADE DO CAMIAO A CRIAR?" << endl;
	cin.clear();
	cin >> capacidade;

	Truck c(id, capacidade);
	fleet.push_back(c);
}

void Company::DeleteTruck()
{
	int id;
	system("cls");
	cout << "--ELIMINAR CAMIAO--" << endl << endl;
	cout << "QUAL O ID DO CAMIAO A ELIMINAR?" << endl;
	cin.clear();
	cin >> id;
	vector<Truck>::iterator it;
	for (it = fleet.begin(); it != fleet.end(); it++)
	{
		if( it->getID() == id)
		{
			fleet.erase(it);
			cout << "CAMIAO " << id << " ELIMINADO COM SUCESSO (PRESSIONE QUALQUER TECLA PARA VOLTAR)" << endl;
			cin.ignore();
			getchar();
			return;
		}
	}
	cout << "NAO EXISTE NENHUM CAMIAO " << id << " (PRESSIONE QUALQUER TECLA PARA VOLTAR)" << endl;
	cin.ignore();
	getchar();
	return;
}

void Company::ShowTrucks()
{
	int n;
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
		cin.clear();
		cin >> n;

	} while (n < 1 || n > 3);

	if (n == 1)
		CreateNewTruck();
	else if (n == 2)
		DeleteTruck();
}

void Company::CreateNewItem()
{
	string nomeDestinatario;
	double valor;
	double peso;
	int nrFatura, id;

	system("cls");
	cout << "--CRIAR NOVO ITEM--" << endl << endl;
	cout << "QUAL O NUMERO DE FATURA DO ITEM?" << endl;
	cin.clear();
	cin >> nrFatura;
	for (unsigned int i = 0; i < itens.size(); i++)
	{
		if( itens[i].getNrFatura() == nrFatura)
		{
			cout << "JA EXISTE UM ITEM COM ESSE NUMERO DE FACTURA (PRESSIONE QUALQUER TECLA PARA VOLTAR)" << endl;
			cin.ignore();
			getchar();
			return;
		}
	}

	cout << "QUAL O ID DO DESTINO?" << endl;
	cin.clear();
	cin >> id;
	Destination d = Destination("", 0, 0, id);
	Vertex<Destination> * v = destinos.getVertex(d);
	if (v == NULL)
	{
		cout << "NAO EXISTE NENHUM DESTINO COM ESSE ID (PRESSIONE QUALQUER TECLA PARA VOLTAR)" << endl;
		cin.ignore();
		getchar();
		return;
	}

	cout << "QUAL O NOME DO DESTINATARIO?" << endl;
	cin.clear();
	cin >> nomeDestinatario;

	cout << "QUAL O VALOR DO ITEM?" << endl;
	cin.clear();
	cin >> valor;

	cout << "QUAL O PESO DO ITEM?" << endl;
	cin.clear();
	cin >> peso;

	Item i(nomeDestinatario, valor, peso, nrFatura, id);
	itens.push_back(i);
}

void Company::DeleteItem()
{
	int n;
	system("cls");
	cout << "--ELIMINAR ITEM--" << endl << endl;
	cout << "QUAL O NUMERO DE FACTURA DO ITEM A ELIMINAR?" << endl;
	cin.clear();
	cin >> n;
	vector<Item>::iterator it;
	for (it = itens.begin(); it != itens.end(); it++)
	{
		if( it->getNrFatura() == n)
		{
			itens.erase(it);
			cout << "ITEM COM FACTURA " << n << " ELIMINADO COM SUCESSO (PRESSIONE QUALQUER TECLA PARA VOLTAR)" << endl;
			cin.ignore();
			getchar();
			return;
		}
	}
	cout << "NAO EXISTE NENHUM ITEM COM FACTURA " << n << " (PRESSIONE QUALQUER TECLA PARA VOLTAR)" << endl;
	cin.ignore();
	getchar();
	return;
}

void Company::ShowItems()
{
	int n;
	system("cls");
	if (itens.size() != 0)
	{
		for (unsigned int i = 0; i < itens.size(); i++)
		{
			cout << "---------------------------------------------------" << endl;
			cout << "Item com fatura numero " << itens[i].getNrFatura() << endl;
			cout << "Destinatario: " << itens[i].getDestinatario() << endl;
			cout << "ID do destino: " << itens[i].getDestino() << endl;
			cout << "Peso: " << itens[i].getPeso() << endl;
			cout << "Valor: " << itens[i].getValor() << endl;
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
		cin.clear();
		cin >> n;
	} while (n < 1 || n > 3);

	if (n == 1)
		CreateNewItem();
	else if (n == 2)
		DeleteItem();
}

void Company::ShowDeliveryPoints()
{
	GraphViewer *gv = new GraphViewer(mapWidth, mapHeight, false);

	gv->setBackground(mapImg);
	gv->createWindow(1200, 900);
	gv->defineEdgeDashed(true);

	int ay = mapHeight/(minLat - maxLat);
	int by = mapHeight - ay*minLat;
	int ax = mapWidth/(maxLong - minLong);
	int bx = mapWidth - ax*maxLong;

	for (unsigned int i = 0; i < itens.size(); i++)
	{
		Destination d = Destination("", 0, 0, itens[i].getDestino() );
		Vertex<Destination> * v = destinos.getVertex(d);
		int id = v->getInfo().getID();
		double lat = v->getInfo().getLatitude();
		double longitude = v->getInfo().getLongitude();
		int y = ay*lat+by;
		int x = ax*longitude+bx;
		gv->addNode(id, x, y);

		std::ostringstream stm ;
		stm << "ITEM #"<< itens[i].getNrFatura();
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

	vector<Vertex<Destination> *> pontos = destinos.getVertexSet();

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
		cout << "GARAGEM E/OU DEPOSITO NAO ESTAO DEFINIDOS" << endl << "(PRESSIONE QUALQUER TECLA PARA VOLTAR)" << endl;
		cin.ignore();
		getchar();
		return;
	}
	
	// vector ordenado por ordem decrescente
	vector<Item> items = itens;
	sort(items.begin(), items.end(), cmpItems);

	vector<Item> itemsToBig;
	vector<Item> itemsNotLinked;

	if (fleet.size() == 0)
	{
		cout << "A FROTA NAO POSSUI CAMIOES" << endl << "(PRESSIONE QUALQUER TECLA PARA VOLTAR)" << endl;
		cin.ignore();
		getchar();
		return;
	}

	for(unsigned int i=0; i<fleet.size(); i++)
		fleet[i].clearItens();
	sort(fleet.begin(), fleet.end(), sortTrucksCapacity);

	vector<Destination> poi;

	poi.push_back( Destination("", 0, 0, garageID) );
	poi.push_back( Destination("", 0, 0, depotID) );

	for(unsigned int i=0; i<items.size(); i++)
	{
		poi.push_back( Destination("", 0, 0, items[i].getDestino()) );
	}
	Graph<Destination> entregas = destinos.makeGraphWithPOI(poi);
	ShowMap(entregas);

	//Verifica se todos os pontos sao acessiveis da garagem, caso contrario, remove do grafo
	Vertex<Destination> * v = entregas.getVertex( Destination("", 0, 0, garageID) );
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
		cout << "O DEPOSITO NAO ESTA ACESSIVEL DA GARAGEM - DEFINA NOVOS LOCAIS " << endl << "(PRESSIONE QUALQUER TECLA PARA VOLTAR)" << endl;
		cin.ignore();
		getchar();
		return;
	}

	//Verifica ligacao de todos os itens
	vector<Item>::iterator it = items.begin();
	while(it != items.end())
	{
		linked = false;
		for(unsigned int j = 0; j < ed.size(); j++)
		{
			if(it->getDestino() == ed[j].getDestination()->getInfo().getID())
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
			entregas.removeVertex( Destination("", 0, 0, it->getDestino()) );
			itemsNotLinked.push_back( *it );
			items.erase(it);
		} else it++;
	}
	//Remove garagem e deposito do grafo pois ja nao sao precisos
	entregas.removeVertex( Destination("", 0, 0, garageID) );
	entregas.removeVertex( Destination("", 0, 0, depotID) );

	// Começa pelo item maior e tenta coloca-lo no camiao
	// Insere o item mais proximo do ultimo item e que caiba ate nao conseguir colocar mais nenhum item
	for(unsigned int i=0; i<fleet.size(); i++)
	{
		if(items.size() == 0) break;

		while(fleet[i].getCapacidade() < items[0].getPeso() && items.size() > 0)
		{
			itemsToBig.push_back( *items.begin() );
			items.erase(items.begin());
		}
		if(items.size() == 0) break;

		fleet[i].addItem(items[0]);
		items.erase(items.begin());
		bool truck_full = false;

		int n=0;
		while(!truck_full && items.size() != 0)
		{
			Vertex<Destination> * v = entregas.getVertex( Destination("", 0, 0, fleet[i].getItems().at(n).getDestino()) );
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
				vector<Item>::iterator it = items.begin();
				for(it = items.begin(); it < items.end(); it++)
				{
					if(it->getDestino() == edges[j].getDestination()->getInfo().getID())
						break;
				}
				if(it->getPeso() + fleet[i].getCurrentCap() <= fleet[i].getCapacidade())
				{
					fleet[i].addItem(*it);
					items.erase(it);
					break;
				}
				else if(j == edges.size()-1) truck_full = true;	// se mais nenhum item cabe, camiao esta cheio
				j++;
			}
			n++;
		}

		cout << "---------------------------------------------------" << endl;
		cout << "CAMIAO " << fleet[i].getID() << " (Capacidade: " << fleet[i].getCurrentCap() << "/" << fleet[i].getCapacidade() << ")"<< endl << endl;
		vector<Item> itens = fleet[i].getItems();
		for(unsigned int i = 0; i < itens.size(); i++)
		{
			cout << "Item #" << itens[i].getNrFatura() << " (Peso: " << itens[i].getPeso() << ")" << endl;
		}
		cout << endl;
	}
	cout << "---------------------------------------------------" << endl;
	sort(fleet.begin(), fleet.end(), sortTrucksId);

	while( itemsNotLinked.size() > 0)
	{
		cout << "ITEM COM FACTURA " << itemsNotLinked[0].getNrFatura() << " NAO PODE SER ENTREGUE PORQUE ESTA INACESSIVEL" << endl;
		itemsNotLinked.erase(itemsNotLinked.begin());
	}

	while( itemsToBig.size() > 0)
	{
		cout << "ITEM COM FACTURA " << itemsToBig[0].getNrFatura() << " NAO PODE SER TRANSPORTADO POR SER DEMASIADO GRANDE" << endl;
		itemsToBig.erase(itemsToBig.begin());
	}

	while( items.size() > 0)
	{
		cout << "ITEM COM FACTURA " << items[0].getNrFatura() << " NAO PODE SER TRANSPORTADO POR FALTA DE CAPACIDADE DA FROTA" << endl;
		items.erase(items.begin());
	}

	cout << endl << "(PRESSIONE QUALQUER TECLA PARA VOLTAR)" << endl;
	cin.ignore();
	getchar();
	return;
}

void Company::CalcRoutes(bool bruteForce)
{
	if (garageID == -1 || depotID == -1)
	{
		cout << "GARAGEM E/OU DEPOSITO NAO ESTAO DEFINIDOS" << endl << "(PRESSIONE QUALQUER TECLA PARA VOLTAR)" << endl;
		cin.ignore();
		getchar();
		return;
	}

	Destination g("", 0, 0, garageID);
	Destination d("", 0, 0, depotID);

	for(unsigned int i = 0; i < fleet.size(); i++)
	{
		if(fleet[i].getItems().size() > 0)
		{
			fleet[i].calcRoute(g, d, destinos, bruteForce);
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
	cout << "(PRESSIONE QUALQUER TECLA PARA VOLTAR)" << endl;
	cin.ignore();
	getchar();
}

void Company::CalcRoutesBruteForce()
{
	CalcRoutes(true);
}

void Company::CalcRoutesGreedy()
{
	CalcRoutes(false);
}

bool Company::setGarage(int idNew)
{
	if ( NULL == destinos.getVertex(Destination("",0,0,idNew)) )
		return false;
	garageID = idNew;
	return true;
}

bool Company::setDepot(int idNew)
{
	if ( NULL == destinos.getVertex(Destination("",0,0,idNew)) )
		return false;
	depotID = idNew;
	return true;
}

void Company::defineGarageDepot()
{
	int id;
	system("cls");
	cout << "INDIQUE O ID DO DESTINO COMO GARAGEM" << "\n";
	cin.clear();
	cin >> id;

	if( !setGarage(id) )
	{
		cout << "ID DO DESTINO INVALIDO (PRESSIONE QUALQUER TECLA PARA VOLTAR)" << "\n";
		cin.ignore();
		getchar();
		return;
	}

	cout << "INDIQUE O ID DO DESTINO COMO DEPOSITO" << "\n";
	cin.clear();
	cin >> id;

	if( !setDepot(id) )
	{
		cout << "ID DO DESTINO INVALIDO (PRESSIONE QUALQUER TECLA PARA VOLTAR)" << "\n";
		cin.ignore();
		getchar();
		return;
	}

	cout << "GARAGEM E DEPOSITO DEFINIDOS COM SUCESSO (PRESSIONE QUALQUER TECLA PARA VOLTAR)" << "\n";
	cin.ignore();
	getchar();
}

void Company::MainMenu()
{
	bool run = true;

	while(run)
	{
		int opt = -1;

		do {
			cout << "BEM-VINDO AO PROGRAMA DE GESTAO DE ENTREGAS NO PORTO" << "\n";
			cout << "\n";
			cout << "1 - VER MAPA" << "\n";
			cout << "2 - DEFINIR GARAGEM E DEPOSITO" << "\n";
			cout << "3 - GERIR CAMIOES" << "\n";
			cout << "4 - GERIR ITENS" << "\n";
			cout << "5 - VER PONTOS DE ENTREGA" << "\n";
			cout << "6 - DISTRIBUIR ITENS PELOS CAMIOES" << "\n";
			cout << "7 - CALCULAR ROTAS DE ENTREGA (SOLUCAO OPTIMA)" << "\n";
			cout << "8 - CALCULAR ROTAS DE ENTREGA (SOLUCAO RAPIDA)" << "\n";
			cout << "0 - SAIR" << "\n";

			cin.clear();
			cin >> opt;

		} while (opt < 0 || opt > 8);

		switch (opt) {
		case 1:
			ShowMap(destinos);
			system("cls");
			break;
		case 2:
			defineGarageDepot();
			system("cls");
			break;
		case 3:
			ShowTrucks();
			system("cls");
			break;
		case 4:
			ShowItems();
			system("cls");
			break;
		case 5:
			ShowDeliveryPoints();
			system("cls");
			break;
		case 6:
			AssignItemsToTrucks();
			system("cls");
			break;
		case 7:
			CalcRoutesBruteForce();
			system("cls");
			break;
		case 8:
			CalcRoutesGreedy();
			system("cls");
			break;
		case 0:
			run = false;
			break;
		default:
			break;
		}
	}
}

