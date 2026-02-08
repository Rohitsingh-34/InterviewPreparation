#include <iostream>
#include <unordered_map>

using namespace std;

/*
Vending Machine LLD without using the STATE DESIGN PATTERN 

*/
enum State {
	NOMONEYSTATE,
	MONEYINSERTEDSTATE,
	DESPENSESTATE
};

//PRODUCT CLASS 
class Product {
private:
	string Sku;
	string name;
	int price;
public:
	Product(string sku, string name, int price) {
		this->Sku = sku;
		this->name = name;
		this->price = price;
	}
	string getProductName() {
		return name;
	}
	int getProductPrice() {
		return price;
	}
};

//RACK CLASS CONTAINS PRODUCT OBJECT;

class Rack {
private:
	int rackNumber;
	Product* product;
	int capacity;
	int stock;
public:
	Rack(int rackNumber, Product* product, int capacity, int stock) {
		this->rackNumber = rackNumber;
		this->product = product;
		this->capacity = capacity;
		this->stock = stock;
	}
	int getRackNumber() {
		return rackNumber;
	}
	Product* getProduct() {
		return product;
	}
	int getStock() {
		return stock;
	}

	void addStock(Product* product) {
		this->product = product;
		stock = capacity;
	}

	void despenseProduct() {
		stock--;
		cout << "stock updated: " << stock << " for product" << product->getProductName() << endl;
	}
};

/// PAYMENT STRATEGY

class IPaymentStrategy {
public:
	virtual int makePayment(int price, int amount) = 0;
	//virtual ~IPaymentStrategy() = default;
};


class DespenseService {
private:
	unordered_map<int,Rack*> racks;
	IPaymentStrategy* paymentStrategy;
	State state;
	int balance;
public:
	DespenseService(unordered_map<int, Rack*> racks, IPaymentStrategy* paymentStrategy, State state = State::NOMONEYSTATE) {
		this->racks = racks;
		this->paymentStrategy = paymentStrategy;
		this->state = state;
		this->balance = 0;
	}
    void setPaymentStrategy(IPaymentStrategy* paymentStrategy) {
        this->paymentStrategy = paymentStrategy;
    }
	Product* selectProduct(int rackNumber) {
		return racks[rackNumber]->getProduct();
	}
	int makePayment(int rackNumber, int amount) {
		if (state == State::NOMONEYSTATE) {
			state = State::MONEYINSERTEDSTATE;
			Product* product = racks[rackNumber]->getProduct();
			int res = paymentStrategy->makePayment(product->getProductPrice(), amount);
			state = State::DESPENSESTATE;
			this->despenseProduct(rackNumber);
			return res;
		}
		else {
			cout << "Machine currently in process" << endl;
			return amount;
		}
	}

	void updateBalance(int price) {
		balance += price;
	}

	void addStock(int rackNumber, Product* product) {
		auto it = racks.find(rackNumber);
		if (it == racks.end()) {
			cout << "racks does not exists" << endl;
			return;
		}

		it->second->addStock(product);
	}

	void despenseProduct(int rackNumber) {
		racks[rackNumber]->despenseProduct();
	}

};

class CashPaymentStrategy : public IPaymentStrategy {
private:
	DespenseService* despenseService;

public:
	CashPaymentStrategy(DespenseService* despenseService) {
		this->despenseService = despenseService;
	};

	int makePayment(int price, int amount) override {
		if (price <= amount) {
			despenseService->updateBalance(price);
			cout << "payment done successfully! amount: " << amount << " ,price: " << price << endl;
			cout << "change returned - " << amount - price << endl;
			return amount - price;
		}
		else{
			cout << "Error: amount is less than price." << endl;
			return -1;
		}
	}
};

int main() {

	int NoofRacks = 3;
	int rackCap = 6;
	unordered_map<int, Rack*> racks;
	racks[101] = new Rack(101, new Product("LYS", "Lays", 40), rackCap, rackCap);
	racks[102] = new Rack(102, new Product("PPS", "Papsi", 60), rackCap, rackCap);
	racks[103] = new Rack(103, new Product("WTR", "Water", 30), rackCap, rackCap);
	racks[104] = new Rack(104, new Product("PRTBR", "Protien Bar", 80), rackCap, rackCap);

    IPaymentStrategy* paymentstg = nullptr;
    DespenseService* despenseService = new DespenseService(racks, paymentstg);

    paymentstg = new CashPaymentStrategy(despenseService);
    despenseService->setPaymentStrategy(paymentstg);

	Product* product = despenseService->selectProduct(102);
	cout << "price of the product - " << product->getProductName() << " : " << product->getProductPrice()<<endl;

	int amount = 100;

	int change1 = despenseService->makePayment(102, amount);
	int change2 = despenseService->makePayment(103, amount);

	cout << "amount paid - " << amount << " , change recieved: " << change1 << endl;
	cout << "amount paid - " << amount << " , change recieved: " << change2<< endl;
	cout << "product recieved" << product->getProductName() << endl;
	return 0;
}


