#include "Company.h"

int main()
{
	system("cls");
	Company t;
	t.ReadMapInfo();
	t.LoadGarageDepot();
	t.LoadCamioes();
	t.LoadItens();
	t.MainMenu();
	t.SaveCamioes();
	t.SaveItens();
	t.SaveGarageDepot();
	
	return 0;
}
