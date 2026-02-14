#include <iostream>
#include <unordered_map>
using namespace std;

class HasCardState;
class ATMState;
class IdleState;

enum class Operation {
	CHECKBALANCE,
	WITHDRAWAL,
	TRANSFER
};


class BankAccount {
private:
	int accountNumber;
	int balance;
public:
	BankAccount(int balance, int accountNumber = 1234) {
		this->balance = balance;
		this->accountNumber = accountNumber;
	}
	int getBalance() {
		return balance;
	}

	void deductBalance(int amount) {
		if (balance >= amount)
			balance = balance - amount;
		else
			cout << "Can't deduct bank balance. Balance is less" << endl;
		return;

	};
};

class Card {
private:
	int cardNumber;
	int PINNumber;
	string customerName;
	BankAccount* bankaccount;
public:
	Card(int cardNumber, int PINNumber, string customerName, BankAccount* bankaccount) {
		this->cardNumber = cardNumber;
		this->bankaccount = bankaccount;
		this->PINNumber = PINNumber;
		this->customerName = customerName;
	}
	int getPinNumber() {
		return PINNumber;
	}
	int setPinNumber(int pin) {
		PINNumber = pin;
	}

	BankAccount* getBankAccount() {
		return bankaccount;
	}
};

class User {
private:
	Card* card;
	BankAccount* bankaccount;

public:
	User(Card* card) {
		this->card = card;
		this->bankaccount = card->getBankAccount();
	}
	Card* getCard() {
		return card;
	}
	void setCard(Card* card) {
		this->card = card;
	}
};


class ATM {
private:
	static ATM* atmobj;
	ATMState* currentState;
	int atmBalance;
	int twoThousandNotes;
	int fiveHundredNotes;
	int hundredNotes;

	ATM() {}

	//ATM(int balance, int twoThousandNotes, int fiveHundredNotes, int hundredNotes) {
	//	this->atmBalance = balance;
	//	this->twoThousandNotes = twoThousandNotes;
	//	this->fiveHundredNotes = fiveHundredNotes;
	//	this->hundredNotes = hundredNotes;
	//}
public:
	//singleton design pattern
	//static ATM* getAtmObj(int balance, int twoThousandNotes, int fiveHundredNotes, int hundredNotes) {
	//	if (atmobj != nullptr)
	//		return atmobj;
	//	atmobj = new ATM(balance, twoThousandNotes, fiveHundredNotes, hundredNotes);
	//	return atmobj;
	//}
	static ATM* getATMObj() {
		if (atmobj == nullptr)
			atmobj = new ATM();
		return atmobj;
	}
void setATMObj(int balance, int twoThousandNotes, int fiveHundredNotes, int hundredNotes) {
       this->atmBalance = balance;
       this->twoThousandNotes = twoThousandNotes;
       this->fiveHundredNotes = fiveHundredNotes;
       this->hundredNotes = hundredNotes;
}
	ATMState* getCurrentState() {
		return currentState;
	}
	void setCurrentState(ATMState* state) {
		this->currentState = state;
	}
	int getATMBalance() {
		return atmBalance;
	}
	int getTwoThousandNotes() {
		return twoThousandNotes;
	}
	int getFiveHundredNotes() {
		return fiveHundredNotes;
	}
	int getHundredNotes() {
		return hundredNotes;
	}

	void deductATMBalance(int amount) {
		atmBalance = atmBalance - amount;
	}

	void deductTwoThousandNotes(int notes) {
		twoThousandNotes -= notes;
	}
	void deductFiveHundredNotes(int notes) {
		fiveHundredNotes -= notes;;
	}
	void deductHundredNotes(int notes) {
		hundredNotes -= notes;
	}

	void printATMBalance() {
		cout << "ATM Balance: Rs " << atmBalance << endl;
		cout << "Rs 2000 notes: " << twoThousandNotes << endl;
		cout << "Rs 500 notes: " << fiveHundredNotes << endl;
		cout << "Rs 100 notes: " << hundredNotes << endl;
	}
};

ATM* ATM::atmobj = nullptr; //assign static variable

// CHAIN OF RESPONSIBILITY DESGIN PATTERN
//Cash dispenser 

class CashHandler {
protected:
	CashHandler* next;
public:
	CashHandler(CashHandler* cashHandler) :next(cashHandler) {}

	virtual bool Withdraw(ATM* atmobj, int amount, unordered_map<int, int>& plan) {
		if (next != nullptr)
			return next->Withdraw(atmobj, amount, plan);
		else
			false;
	}
};

class TwoThousandCashHandler : public CashHandler {
public:
	TwoThousandCashHandler(CashHandler* cashHandler) : CashHandler(cashHandler) {};

	bool Withdraw(ATM* atmobj, int amount, unordered_map<int, int>& plan) override {
		int remAmount = amount % 2000;
		int notes = amount / 2000;

		if (notes <= atmobj->getTwoThousandNotes())
			plan[2000] += notes;
		else if (notes > atmobj->getTwoThousandNotes()) {
			plan[2000] += atmobj->getTwoThousandNotes();
			remAmount = amount - (atmobj->getTwoThousandNotes()) * 2000;
		}
		//deduct amount;

		if (remAmount != 0)
			return CashHandler::Withdraw(atmobj, remAmount, plan);
		else
			return true;
	}
};

class FiveHundredCashHandler : public CashHandler {
public:
	FiveHundredCashHandler(CashHandler* cashHandler) : CashHandler(cashHandler) {};

	bool Withdraw(ATM* atmobj, int amount, unordered_map<int, int>& plan) override {
		int remAmount = amount % 500;
		int notes = amount / 500;

		if (notes <= atmobj->getFiveHundredNotes())
			plan[500] += notes;
		else if (notes > atmobj->getFiveHundredNotes()) {
			plan[500] += atmobj->getFiveHundredNotes();
			remAmount = amount - (atmobj->getFiveHundredNotes()) * 500;
		}
		//deduct amount;

		if (remAmount != 0)
			return CashHandler::Withdraw(atmobj, remAmount, plan);
		else
			return true;
	}
};

class HundredCashHandler : public CashHandler {
public:
	HundredCashHandler(CashHandler* cashHandler) : CashHandler(cashHandler) {};

	bool Withdraw(ATM* atmobj, int amount, unordered_map<int, int>& plan) override {
		int remAmount = amount % 100;
		int notes = amount / 100;
		if (notes <= atmobj->getHundredNotes())
			plan[100] += notes;
		else if (notes > atmobj->getHundredNotes()) {
			plan[100] += atmobj->getHundredNotes();
			remAmount = amount - (atmobj->getHundredNotes()) * 100;
		}
		//deduct amount;
		if (remAmount != 0)
			return CashHandler::Withdraw(atmobj, remAmount, plan);
		return true;
	}
};

//STATE DESGIN PATTERN

class ATMState {
public:
	virtual void InsertCard(ATM* atmobj) { cout << "Oops: something went wrong!!" << endl; };

	virtual void AuthenticatePIN(ATM* atmobj, Card* card, int pin) { cout << "Oops: something went wrong!!" << endl; };

	virtual void SelectOperation(ATM* atmobj, Operation operation) { cout << "Oops: something went wrong!!" << endl; };

	virtual int CheckBalance(ATM* atmobj, Card* card) { cout << "Oops: something went wrong!!" << endl; };

	virtual int WithdrawBalance(ATM* atmobj, Card* card, int amount) { cout << "Oops: something went wrong!!" << endl; };

	virtual void ReturnCard() { cout << "Oops: something went wrong!!" << endl; };

	virtual void Exits(ATM* atmobj) { cout << "Oops: something went wrong!!" << endl; };
};


class IdleState : public ATMState {
public:
	void InsertCard(ATM* atmobj);
};


class CheckBalanceState : public ATMState {

	int CheckBalance(ATM* atmobj, Card* card) override {
		atmobj->setCurrentState(new IdleState());
		cout << "current account balance is - Rs " << card->getBankAccount()->getBalance() << endl;
		return card->getBankAccount()->getBalance();
	}
	void Exits(ATM* atmobj) {
		ReturnCard();
		atmobj->setCurrentState(new IdleState());
		cout << "users exits!!" << endl;
		return;
	}
	void ReturnCard() {
		cout << "Card returned!!" << endl;
		return;
	}
};

class CashWidrawalState : public ATMState {

	int WithdrawBalance(ATM* atmobj, Card* card, int amount) override {
		if (amount > card->getBankAccount()->getBalance())
			cout << "Bank balance dont have sufficient balance" << endl;
		else if (amount > atmobj->getATMBalance())
			cout << "ATM dont have sufficient balance" << endl;
		else
		{
			CashHandler* handler = new TwoThousandCashHandler(new  FiveHundredCashHandler(new HundredCashHandler(nullptr)));
			unordered_map<int, int> plan;
			bool canDeduct = handler->Withdraw(atmobj, amount, plan);
			if (canDeduct) {
				cout << "deducted denominations" << endl;
				cout << "2000-" << plan[2000] << ", 500-" << plan[500] << ", 100-" << plan[100] << endl;
				atmobj->deductTwoThousandNotes(plan[2000]);
				atmobj->deductFiveHundredNotes(plan[500]);
				atmobj->deductHundredNotes(plan[100]);
				atmobj->deductATMBalance(amount);
				cout << "transaction completed successfully" << endl;
				card->getBankAccount()->deductBalance(amount);
			}
			else
				cout << "sufficient denomitaions not avaiable" << endl;
			atmobj->setCurrentState(new IdleState());
		}
		return 0;
	}

	void Exits(ATM* atmobj) {
		ReturnCard();
		atmobj->setCurrentState(new IdleState());
		cout << "users exits!!" << endl;
		return;
	}
	void ReturnCard() {
		cout << "Card returned!!" << endl;
		return;
	}
};

class SelectOperationState : public ATMState {

	void SelectOperation(ATM* atmobj, Operation operation) override {
		switch (operation) {
		case Operation::WITHDRAWAL:
			cout << "Withdrawal option is choosen" << endl;
			atmobj->setCurrentState(new CashWidrawalState());
			break;

		case Operation::CHECKBALANCE:
			cout << "CheckBalance option is choosen" << endl;
			atmobj->setCurrentState(new CheckBalanceState());
			break;

		default:
			cout << "Choosen options is not correct!" << endl;
			break;
		}
		return;
	}
	void Exits(ATM* atmobj) {
		ReturnCard();
		atmobj->setCurrentState(new IdleState());
		cout << "users exits!!" << endl;
		return;
	}
	void ReturnCard() {
		cout << "Card returned!!" << endl;
		return;
	}
};

class HasCardState : public ATMState {
public:
	void AuthenticatePIN(ATM* atmobj, Card* card, int pin) override {
		if (card->getPinNumber() == pin) {
			cout << "Card is authenticated." << endl;
			atmobj->setCurrentState(new SelectOperationState());
		}
		else {
			cout << "Wrong PIN Number is Inserted. Please Retry!!" << endl;
		}
		return;
	}
	void Exits(ATM* atmobj) {
		ReturnCard();
		atmobj->setCurrentState(new IdleState());
		cout << "users exits!!" << endl;
		return;
	}
	void ReturnCard() {
		cout << "Card returned!!" << endl;
		return;
	}
};

//forward declaration method implementation
void IdleState::InsertCard(ATM* atmobj) {
	cout << "Card is inerted into ATM" << endl;
	atmobj->setCurrentState(new HasCardState());
}

class ATMRoom {
public:
	ATM* atmobj;
	User* user;
	ATMRoom() {}
	void initilizeATM(int balance, int twoThousandNotes, int fiveHundredNotes, int hundredNotes) {
		//		atmobj = atmobj->getAtmObj(balance, twoThousandNotes, fiveHundredNotes, hundredNotes);
			/*	atmobj = ATM::getAtmObj(balance, twoThousandNotes, fiveHundredNotes, hundredNotes);
				atmobj->setCurrentState(new IdleState());*/

		atmobj = ATM::getATMObj();
		atmobj->setATMObj(balance, twoThousandNotes, fiveHundredNotes, hundredNotes);
		atmobj->setCurrentState(new IdleState()); // Set state here, after IdleState is defined
		user = new User(this->createCard());
	}
	ATM* getATMobj() {
		return atmobj;
	}
	Card* createCard() {
		return new Card(1234, 1111, "john", createBankAccount(9000));
	}
	BankAccount* createBankAccount(int balance) {
		return new BankAccount(balance);
	}

};

int main() {
	ATMRoom* atmroom = new ATMRoom();
	atmroom->initilizeATM(7000, 2, 4, 10);
	atmroom->atmobj->printATMBalance();
	atmroom->getATMobj()->getCurrentState()->InsertCard(atmroom->atmobj);
	atmroom->getATMobj()->getCurrentState()->AuthenticatePIN(atmroom->atmobj, atmroom->user->getCard(), 1111);
	//	atmroom->atmobj->getCurrentState()->SelectOperation(atmroom->atmobj, Operation::CHECKBALANCE);
		//atmroom->atmobj->getCurrentState()->CheckBalance(atmroom->atmobj, atmroom->user->getCard());
	atmroom->atmobj->getCurrentState()->SelectOperation(atmroom->atmobj, Operation::WITHDRAWAL);
	atmroom->atmobj->getCurrentState()->WithdrawBalance(atmroom->atmobj, atmroom->user->getCard(), 3100);
	atmroom->atmobj->printATMBalance();

	atmroom->getATMobj()->getCurrentState()->InsertCard(atmroom->atmobj);
	atmroom->getATMobj()->getCurrentState()->AuthenticatePIN(atmroom->atmobj, atmroom->user->getCard(), 1111);
	//	atmroom->atmobj->getCurrentState()->SelectOperation(atmroom->atmobj, Operation::CHECKBALANCE);
		//atmroom->atmobj->getCurrentState()->CheckBalance(atmroom->atmobj, atmroom->user->getCard());
	atmroom->atmobj->getCurrentState()->SelectOperation(atmroom->atmobj, Operation::WITHDRAWAL);
	atmroom->atmobj->getCurrentState()->WithdrawBalance(atmroom->atmobj, atmroom->user->getCard(), 3370);

	atmroom->atmobj->printATMBalance();

}


/*
1. if balance is suffice in atm but domniations not add to that amount //fixed
2. also deduct the notes once done //fixed
3. after withdrawal fails we can also move to select opration state for other amount
*/