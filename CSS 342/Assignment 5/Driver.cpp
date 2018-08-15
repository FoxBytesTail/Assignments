#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include "Bank.h"
#include "Transaction.h"

using namespace std;
int main()
{
	string line;
	ifstream file("BankTransIn.txt");

	Bank b;
	queue <string> transactions;

	while (getline(file, line))
	{
		transactions.push(line);
	}

	file.close();

	while (!transactions.empty())
	{
		b.Process(transactions.front());
		transactions.pop();
	}

	std::cout << endl << "Processing Done. Final Balances" << endl;
	b.DisplayAccounts();

	//std::cout << "Destroying" << endl;
	b.DestroyBST();
	//std::cout << "Destroyed" << endl;
	//b.DisplayAccounts();

	//cin.get();
	return 0;
}