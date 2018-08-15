#pragma once
#include <string>
#include <vector>

using namespace std;

class Transaction
{
public:
	Transaction();
	Transaction(int _fund, int amount, char _type, bool _success);
	Transaction(int _fund, int _other, int _amount, char _type, bool _success);
	int getFund();
	int getAmount();
	int getDestinationAccount();
	char getType();
	bool getSuccess();
	string getFundName();
	~Transaction();

private:
	int fund;
	bool success;
	int amount;
	int destinationAccount; // other account_id (for fund transfers)
	char type;
};