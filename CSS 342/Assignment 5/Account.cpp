#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>
#include "Account.h"
#include "Transaction.h"

using namespace std;

Account::Account() {}
Account::~Account() {}

int Account::getAccountId()
{
	return id;
}

Account::Account(int aid, string _lastName, string _firstName)
{
	id = aid;
	firstName = _firstName;
	lastName = _lastName;
}

void Account::AddHistoryEntry(Transaction txn)
{
	history[txn.getFund()].push_back(txn);
}

void Account::AddHistoryEntry(Transaction txn, int associateToFund)
{
	history[associateToFund].push_back(txn);
}

vector<Transaction> Account::GetHistoryEntry(int fundId)
{
	return history[fundId];
}

int Account::Balance(int fund)
{
	return balances[fund];
}

string Account::getName()
{
	ostringstream o;
	o << firstName << " " << lastName;
	return o.str();
}

void Account::Increment(int amount, int fund)
{
	if (amount < 0) { return; }
	balances[fund] = balances[fund] + amount;
}

void Account::Decrement(int amount, int fund)
{
	if (amount < 0) { return; }
	balances[fund] = balances[fund] - amount;
}