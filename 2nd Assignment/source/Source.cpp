#include "Company.h"


int main()
{
	Company t;

	t.ReadMapInfo();
	t.LoadGarageDepot();
	t.LoadTrucks();
	t.LoadItems();
	t.MainMenu();
	t.SaveTrucks();
	t.SaveItems();
	t.SaveGarageDepot();

	return 0;
}
