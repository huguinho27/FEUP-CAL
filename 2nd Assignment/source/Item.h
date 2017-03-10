#pragma once
#include "Destination.h"
#include <iostream>

using namespace std;

class Item
{
private:
	string recipientName;
	double value;
	double weight;
	int idDest;
	int invoiceNumber;
public:
	Item();
	Item(string nomeDestinatario, double valor, double peso, int nrFatura, int idDest);
	string getRecipient() const;
	double getValue() const;
	double getWeight() const;
	int getDestinationID() const;
	int getInvoiceNumber() const;
	bool operator<(const Item& right) const;
};
