#pragma once
#include <string>
#include <vector>

#include "BSTree.h"
#include "Transaction.h"
#include "Account.h"

using namespace std;

static vector <string> tokenize(string s);
class Bank
{
public:
	Bank();
	~Bank();

	void DisplayAccounts();
	void Process(string strToParse);
	void DestroyBST();

	void GetHistory(int aid);
	void GetBalances(int aid);
	void GetHistory(int aid, int fund);

	bool AccountExists(int aid);

private:
	void Open(string last, string first, int aid);
	void Deposit(int aid, int amount);
	void Withdraw(int aid, int amount);
	void Transfer(int aid1, int aid2, int amount);

	std::pair<int, int> getAccountIDAndFund(int aid);
	BSTree accounts;
};