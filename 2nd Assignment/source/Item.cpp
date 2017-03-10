#include "Item.h"
using namespace std;

Item::Item(){}

Item::Item(string nomeDestinatario, double valor, double peso, int nrFatura, int idDest)
{
	this->recipientName = nomeDestinatario;
	this->value = valor;
	this->weight = peso;
	this->invoiceNumber = nrFatura;
	this->idDest = idDest;
}

string Item::getRecipient() const
{ return this->recipientName; }

double Item::getValue() const
{ return this->value; }

double Item::getWeight() const
{ return this->weight; }

int Item::getDestinationID() const
{ return this->idDest; }

int Item::getInvoiceNumber() const
{ return this->invoiceNumber; }

bool Item::operator<(const Item &right) const
{
	return (weight < right.getWeight());
}
