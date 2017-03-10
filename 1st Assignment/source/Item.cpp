#include "Item.h"
using namespace std;

Item::Item(){}

Item::Item(string nomeDestinatario, double valor, double peso, int nrFatura, int idDest)
{
	this->nomeDestinatario = nomeDestinatario;
	this->valor = valor;
	this->peso = peso;
	this->nrFatura = nrFatura;
	this->idDest = idDest;
}

string Item::getDestinatario() const
{ return this->nomeDestinatario; }

double Item::getValor() const
{ return this->valor; }

double Item::getPeso() const
{ return this->peso; }

int Item::getDestino() const
{ return this->idDest; }

int Item::getNrFatura() const
{ return this->nrFatura; }

bool Item::operator<(const Item &right) const
{
	return (peso < right.getPeso());
}
