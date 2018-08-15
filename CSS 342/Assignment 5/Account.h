#pragma once
#include <vector>
#include <string>
#include "Transaction.h"
using namespace std;

class Account
{
public:
	Account(int aid, string _lastName, string _firstName);
	void AddHistoryEntry(Transaction txn);
	void AddHistoryEntry(Transaction txn, int associateToFund);
	vector<Transaction> GetHistoryEntry(int fundId);
	int Balance(int fund);
	string getName();
	void Increment(int amount, int fund);
	Account();
	~Account();
	int getAccountId();
	void Decrement(int amount, int fund);
private:
	int id;
	int balances[10] = { 0 };
	string firstName;
	string lastName;
	vector<Transaction> history[10];
};