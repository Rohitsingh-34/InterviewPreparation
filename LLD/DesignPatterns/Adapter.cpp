#include <iostream>

//ADAPTER DESIGN PATTERN

// Old method
class PayInINR
{
public:
    void pay(int inrAmount)
    {
        std::cout << "amount paid in INR - " << inrAmount << std::endl;
    }
};

//interface
class PayBill
{
public:
    virtual void pay(int amount) = 0;
    virtual ~PayBill() = default;
};

//new method
class PayInUsd : public PayBill
{
private:
    PayInINR* payInr;

public:
    PayInUsd()
        : payInr(new PayInINR())
    {
    }

    ~PayInUsd() override
    {
        delete payInr;
    }

    void pay(int amount) override
    {
        std::cout << "payment initiated in dollar for amount: " << amount << std::endl;
        payInr->pay(amount * 90);
    }
};

int main()
{
    PayBill* payUsd = new PayInUsd();
    payUsd->pay(50);
    delete payUsd;
    return 0;
}           