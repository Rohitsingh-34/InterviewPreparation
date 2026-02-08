#include <iostream>
#include <unordered_map>

using namespace std;



// ===================== INTERFACES =====================

class IPaymentStrategy {
public:
    virtual int makePayment(int price, int amount) = 0;
};

class IState {
public:
    virtual IState* selectProduct(DespenseService* ds, int rackNumber) = 0;
    virtual IState* makePayment(DespenseService* ds,int rackNo, int amount) = 0;
    virtual IState* despenseProduct(DespenseService* ds, int rackNo) = 0;
    virtual IState* addStock(DespenseService* ds,int rackNo, Product*) = 0;
    virtual ~IState() = default;
};

// ===================== PRODUCT & RACK =====================

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
    string getProductName() const {
        return name;
    }
    int getProductPrice() const {
        return price;
    }
};

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
    int getRackNumber() const {
        return rackNumber;
    }
    Product* getProduct() const {
        return product;
    }
    int getStock() const {		
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
// ===================== SERVICE =====================

class DespenseService {
private:
	unordered_map<int, Rack*> racks;
	IPaymentStrategy* paymentStrategy;
	int balance;
	IState* currentState;
	IState* noMoneyState;
	IState* itemSelectedState;
	IState* despenseState;
	IState* soldOutState;
public:
	DespenseService(unordered_map<int, Rack*> racks, IPaymentStrategy* paymentStrategy);

	IState* getCurrentState() {
		return currentState;
	}
	IState* getNoMoneyState() {
		return noMoneyState;
	}
	IState* getItemSelectedState() {
		return itemSelectedState;
	}
	IState* getDespenseState() {
		return despenseState;
	}
	IState* getSoldOutState() {
		return soldOutState;
	};

	IPaymentStrategy* getPaymentStrategy() {
		return paymentStrategy;
	}
	void setPaymentStrategy(IPaymentStrategy* paymentStrategy) {
		this->paymentStrategy = paymentStrategy;
	}
	Product* getProduct(int rackNumber){
		return racks[rackNumber]->getProduct();
	}
	Product* selectProduct(int rackNumber);

	int makePayment(int rackNumber, int amount);


	void updateBalance(int price) {
		balance += price;
	}

	/// THIS IS NOT correct way, we need to introduce it each state, 
	/// ITS BEHAVIOUR DEPEND ON STATE
	void reset() {
		currentState = getNoMoneyState();
	}
	
	unordered_map<int, Rack*>& getRacks() {
		return racks;
	}
	void addStock(int rackNumber, Product* product) {
		auto it = racks.find(rackNumber);
		if (it == racks.end()) {
			cout << "racks does not exists" << endl;
			return;
		}

		it->second->addStock(product);
	}
	void despenseProduct(int rackNumber);
};


// ===================== STATES =====================

class NoMoneyState : public IState {
public:
	IState* selectProduct(DespenseService* ds, int rackNumber) {
		if (ds->getRacks()[rackNumber]->getStock() == 0) {
			cout << "selected product is out of stock" << endl;
			return ds->getCurrentState();
		}
			
		return ds->getItemSelectedState();
	}
	IState* makePayment(DespenseService* ds, int rackNo, int amount) {

		cout << "INVALID: Payment making before selecting product" << endl;

		return ds->getCurrentState();
	};
	IState* despenseProduct(DespenseService* ds, int rackNo) {
		cout << "INVALID: trying to despense product before selecting product" << endl;
		return ds->getCurrentState();
	};
	IState* addStock(DespenseService* ds, int rackNo, Product* product) {
		return ds->getCurrentState();
	};
};

class ItemSelectedState : public IState {
public:
	IState* selectProduct(DespenseService* ds, int rackNumber) {
        cout << "Invalid: product is already selected" << endl;
        return ds->getCurrentState();
    }
	IState* makePayment(DespenseService* ds, int rackNo, int amount) {
        Product* product = ds->getRacks()[rackNo]->getProduct();
        int res = ds->getPaymentStrategy()->makePayment(product->getProductPrice(), amount);
        if (res == -1)
            return ds->getCurrentState();
        return ds->getDespenseState();
	};
	IState* despenseProduct(DespenseService* ds, int rackNo) {
        cout << "INVALID: trying to despense product before making payment" << endl;
        return ds->getCurrentState();
	};
	IState* addStock(DespenseService* ds, int rackNo, Product* product) {
        return ds->getCurrentState();
	};
};

class DespenseState : public IState {
public:
	IState* selectProduct(DespenseService* ds, int rackNumber) {
		cout << "Invalid: product is already selected & is in despensing state" << endl;
		return ds->getCurrentState();
	}
	IState* makePayment(DespenseService* ds, int rackNo, int amount) {

		cout << "INVALID: Payment already made, its in despensing state" << endl;
		return ds->getCurrentState();
	};
	IState* despenseProduct(DespenseService* ds, int rackNo) {
		ds->getRacks()[rackNo]->despenseProduct();
		cout << "stock updated for product" << ds->getProduct(rackNo)->getProductName() << endl;
		for (auto& it : ds->getRacks()) {
			
			if(it.second->getStock() >0 )
				return ds->getNoMoneyState();
		}
		return ds->getSoldOutState();
	};
	IState* addStock(DespenseService* ds, int rackNo, Product* product) {
		return ds->getCurrentState();
	};
};


class SoldOutState : public IState {
public:
	IState* selectProduct(DespenseService* ds, int rackNumber) {
		cout << "Invalid: product is out of stock" << endl;
		return ds->getCurrentState();
	}
	IState* makePayment(DespenseService* ds, int rackNo, int amount) {

		cout << "INVALID: product is out of stock" << endl;
		return ds->getCurrentState();
	};
	IState* despenseProduct(DespenseService* ds, int rackNo) {
		cout << "product is out of stock" << endl;
		return ds->getCurrentState();
	};
	IState* addStock(DespenseService* ds, int rackNo, Product* product) {
		for (auto& it : ds->getRacks()) {
			it.second->addStock(it.second->getProduct());
		}
		return ds->getNoMoneyState();
	};
};


// ===================== PAYMENT STRATEGY =====================

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
		else {
			cout << "Error: amount is less than price." << endl;
			return -1;
		}
	}
};

// ===================== SERVICE IMPLEMENTATION =====================

DespenseService::DespenseService(unordered_map<int, Rack*> racks, IPaymentStrategy* paymentStrategy) {
	this->racks = racks;
	this->paymentStrategy = paymentStrategy;
	this->balance = 0;
	currentState = new NoMoneyState();
	noMoneyState = new NoMoneyState();
	itemSelectedState = new ItemSelectedState();
	despenseState = new DespenseState();
	soldOutState = new SoldOutState();
}

Product* DespenseService::selectProduct(int rackNumber) {
	currentState = currentState->selectProduct(this, rackNumber);
	return 	racks[rackNumber]->getProduct();
}
int DespenseService::makePayment(int rackNumber, int amount) {
	currentState = currentState->makePayment(this, rackNumber, amount);
	despenseProduct(rackNumber);
	return 0;
}

void DespenseService::despenseProduct(int rackNumber) {
	currentState = currentState->despenseProduct(this, rackNumber);
}
// ===================== MAIN =====================

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
	// --- Test harness: multiple scenarios ---
	cout << "===== Vending Machine Test Harness =====" << endl;

	// Scenario 1: Normal purchase (overpay)
	cout << "\n--- Scenario 1: Normal purchase (overpay) ---" << endl;
	{
		int rack = 102;
		Product* product = despenseService->selectProduct(rack);
		if (product) {
			cout << "Selected product: " << product->getProductName()
				<< " | price: " << product->getProductPrice() << endl;
		}
		int amount = 100;
		cout << "Attempting payment of " << amount << " for rack " << rack << endl;
		int change = despenseService->makePayment(rack, amount);
		cout << "Change returned: " << change << endl;
		cout << "Stock after attempt: " << racks[rack]->getStock() << endl;
	}
	// Scenario 2: Exact payment
	cout << "\n--- Scenario 2: Exact payment ---" << endl;
	{
		int rack = 103;
		Product* product = despenseService->selectProduct(rack);
		if (product) {
			cout << "Selected product: " << product->getProductName()
				<< " | price: " << product->getProductPrice() << endl;
		}
		int amount = product ? product->getProductPrice() : 0;
		cout << "Attempting exact payment of " << amount << " for rack " << rack << endl;
		int change = despenseService->makePayment(rack, amount);
		cout << "Change returned: " << change << endl;
		cout << "Stock after attempt: " << racks[rack]->getStock() << endl;
	}
	// Scenario 3: Underpay (should fail / return -1 or print error)
	cout << "\n--- Scenario 3: Underpay (should fail) ---" << endl;
	{
		int rack = 104;
		Product* product = despenseService->selectProduct(rack);
		if (product) {
			cout << "Selected product: " << product->getProductName()
				<< " | price: " << product->getProductPrice() << endl;
		}
		int amount = 50; // less than price (80)
		cout << "Attempting underpayment of " << amount << " for rack " << rack << endl;
		int change = despenseService->makePayment(rack, amount);
		cout << "Change/Result: " << change << " (expected -1 or error)" << endl;
		cout << "Stock after attempt: " << racks[rack]->getStock() << endl;
	}
	despenseService->reset();
	// Scenario 4: Sold-out simulation
	cout << "\n--- Scenario 4: Sold-out simulation ---" << endl;
	{
		int rack = 101;
		cout << "Depleting stock for rack " << rack << " by despensing until 0..." << endl;
		// Keep despensing using the Rack method directly to simulate users taking products
		while (racks[rack]->getStock() > 0) {
			racks[rack]->despenseProduct();
		}
		cout << "Stock now: " << racks[rack]->getStock() << endl;
		cout << "Attempt to select and buy from sold-out rack " << rack << endl;
		Product* product = despenseService->selectProduct(rack);
		if (product) {
			cout << "Selected product (should be out): " << product->getProductName()
				<< " | price: " << product->getProductPrice() << endl;
		}
		int change = despenseService->makePayment(rack, 100);
		cout << "Change/Result: " << change << endl;
		cout << "Stock after attempt: " << racks[rack]->getStock() << endl;
	}
	despenseService->reset();
	// Scenario 5: Restock then purchase
	cout << "\n--- Scenario 5: Restock then purchase ---" << endl;
	{
		int rack = 101;
		cout << "Restocking rack " << rack << " with fresh product instance." << endl;
		despenseService->addStock(rack, new Product("LYS", "Lays", 40));
		cout << "Stock after restock: " << racks[rack]->getStock() << endl;
		Product* product = despenseService->selectProduct(rack);
		if (product) {
			cout << "Selected product: " << product->getProductName()
				<< " | price: " << product->getProductPrice() << endl;
		}
		int amount = 50;
		cout << "Attempting payment of " << amount << " for rack " << rack << endl;
		int change = despenseService->makePayment(rack, amount);
		cout << "Change returned: " << change << endl;
		cout << "Final stock: " << racks[rack]->getStock() << endl;
	}

	cout << "\n===== Test Harness Completed =====" << endl;
	return 0;
}


