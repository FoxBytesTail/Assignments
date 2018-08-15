#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>
#include "Funds.h"
#include "Transaction.h"

using namespace std;
Transaction::Transaction() {}

Transaction::Transaction(int _fund, int _amount, char _type, bool _success)
{
	fund = _fund;
	amount = _amount;
	type = _type;
	success = _success;
}

Transaction::Transaction(int _fund, int _destinationAccount, int _amount, char _type, bool _success)
{
	fund = _fund;
	amount = _amount;
	type = _type;
	success = _success;
	destinationAccount = _destinationAccount;
}

int Transaction::getFund()
{
	return fund;
}
int Transaction::getAmount()
{
	return amount;
}
int Transaction::getDestinationAccount()
{
	return destinationAccount;
}
char Transaction::getType()
{
	return type;
}
bool Transaction::getSuccess()
{
	return success;
}

string Transaction::getFundName()
{
	return FundTypes[fund];
}

Transaction::~Transaction()
{
}
