#pragma once
#include "Destination.h"
#include <iostream>

using namespace std;

class Item
{
private:
	string nomeDestinatario;
	double valor;
	double peso;
	int idDest;
	int nrFatura;
public:
	Item();
	Item(string nomeDestinatario, double valor, double peso, int nrFatura, int idDest);
	string getDestinatario() const;
	double getValor() const;
	double getPeso() const;
	int getDestino() const;
	int getNrFatura() const;
	bool operator<(const Item& right) const;
};
