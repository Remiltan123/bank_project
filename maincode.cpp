#include <iostream>
#include <conio.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <time.h>
#include <fstream>
#include <sstream>

using namespace std;

class Administrator;
class Employee;
class Customer;
class Transaction;

class Bank
{
public:
    vector < Employee > employeeArray;
    vector < string > employeeNameArray;
    vector < Customer > customerArray;
    vector < string > customerNameArray;
    vector <Transaction> transactions;
    long bankBalance = 1000000;
};

class Administrator
{
private:
    string dateToday;
    float annualSavingInterest = 20;
    float overdraftCharge = 2;
    int dateIncremented = 0;

public:
    void create_Employee(Bank& bank);
    void set_Date();
    void increase_Date();
    void set_annualSavingInterest();
    void set_overdraftCharge();
    void update_Customer_Overdraft(Customer & customer);
    void update_Customer_Interest(Customer & customer,Bank& bank,ofstream& BankFile);
    string get_Employee_Name(Bank& bank);
    string get_Date();
    float get_annualSavingInterest();
    float get_overdraftCharge();
    int employee_Number(Bank& bank);
    vector<string> get_Employee_Name_Array(Bank& bank);
};

class Employee
{
private:
    string employeeName;
    string dateToday;
public:
    void set_Date(Administrator & Admin);
    void set_Employee_Name(string employeeName);
    string get_Employee_Name();
    void create_Customer(Customer & customerCreated,Bank& bank,ofstream& MyFile);
    void deposit_Money(Customer & customer,Bank& bank,ofstream& MyFile,ofstream& BankFile);
    void withdraw_Money(Customer & customer,Bank& bank,ofstream& MyFile,ofstream& BankFile);
    void view_Transactions(Bank& bank);
    void close_Customer_Account(Customer & customer,ofstream& MyFile);
};

class Customer
{
public:
    string CustomerName;
    string accountType;
    string customerContact;
    long moneyDeposit;
    bool isCurrentAccount = false;
    long overdraftMaximum = 0;
    bool isClosed = false;
    long overDraftNow = 0;
    string dateToday;
    string dateCreated;
    vector <Transaction> transactions;

    void view_Transaction();
    void deposit_Money_Function(Bank& bank,ofstream& MyFile,ofstream& BankFile);
    void set_Date(Administrator& Admin);
    void withdraw_Money(Bank& bank,ofstream& MyFile,ofstream& BankFile);
};

class Transaction
{
private:
    string dateToday;
    string description;
public:

    void setTransactionDate(string dateToday)
    {
        this->dateToday=dateToday;
    }

    void setDescription(string customer_name,string debitOrCredit,long amount)
    {
        this->description=this->dateToday+" - "+customer_name+" : "+debitOrCredit+" : "+to_string(amount)+" Rs.";
    }
    string getDescription()
    {
        return this->description;
    }

};

int Administrator::employee_Number(Bank& bank)
{
    return bank.employeeArray.size();
};

vector<string> Administrator::get_Employee_Name_Array(Bank& bank)
{
    return bank.employeeNameArray;
};

void Administrator::create_Employee(Bank& bank)
{
    string startLetter="";
    if(bank.employeeArray.size()<9)
    {
        startLetter="00";
    }
    else if(bank.employeeArray.size()<99)
    {
        startLetter="0";
    }
    string employeeName=("EMPLOYEE" +startLetter+to_string((bank.employeeArray.size()) + 1));
    Employee employee;
    employee.set_Employee_Name(employeeName);
    bank.employeeArray.push_back(employee);
    bank.employeeNameArray.push_back(employeeName);
    Employee lastCreatedEmployee=bank.employeeArray.back();
    cout<<lastCreatedEmployee.get_Employee_Name()+" : EMPLOYEE IS CREATED."<<endl;;
};

void Administrator::set_Date()
{
    string dateToday;
    time_t ttime = time(0);
    tm * local_time = localtime( & ttime);
    dateToday = to_string(1900 + local_time -> tm_year) + ":" + to_string(1 + local_time -> tm_mon) + ":" + to_string(local_time -> tm_mday);
    this -> dateToday = dateToday;
};

void Administrator::increase_Date()
{
    this->dateIncremented+=1;
    time_t ttime = time(0);
    ttime += (24 * 60 * 60)*(this->dateIncremented);
    tm * local_time = localtime( & ttime);
    this->dateToday = to_string(1900 + local_time -> tm_year) + ":" + to_string(1+local_time -> tm_mon) + ":" + to_string(local_time -> tm_mday);
};

void Administrator::set_annualSavingInterest()
{
    float annualSavingInterest;
    cout << "ENTER THE NEW ANNUAL SAVING INTEREST : ";
    cin >> annualSavingInterest;
    this -> annualSavingInterest = annualSavingInterest;
    cout<<"NEW ANNUAL SAVINGS INTEREST IS "<< annualSavingInterest<<endl;
};

void Administrator::set_overdraftCharge()
{
    float overdraftCharge;
    cout << "ENTER THE NEW OVERDRAFT CHARGE(% PER DAY) : ";
    cin >> overdraftCharge;
    this -> overdraftCharge = overdraftCharge;
    cout<<"NEW OVERDRAFT CHARGE IS "<< overdraftCharge<<endl;
};

void Administrator::update_Customer_Overdraft(Customer & customer)
{
    if (customer.overDraftNow > 0)
    {
        customer.overDraftNow += ((this -> overdraftCharge) / 100) * (customer.overDraftNow);
        customer.moneyDeposit=(customer.overDraftNow)*(-1);

    }
}

void Administrator::update_Customer_Interest(Customer & customer,Bank& bank,ofstream& BankFile)
{
    float dailyInterest = (this -> annualSavingInterest) / 365;
    if (customer.moneyDeposit > 0)
    {
        cout<<"LINE-1"<<endl;
        customer.moneyDeposit += ((dailyInterest) / 100) * (customer.moneyDeposit);
        bank.bankBalance-=((dailyInterest) / 100) * (customer.moneyDeposit);
        BankFile<<this->dateToday<<" - "<<customer.CustomerName<<" : INTEREST DEBITED TO BANK ACCOUNT : "<<to_string(((dailyInterest) / 100) * (customer.moneyDeposit))<<" Rs."<<endl;
        BankFile<<this->dateToday<<" - "<<"BANK BALANCE : "<<bank.bankBalance<<endl;
        Transaction transaction_now;
        transaction_now.setTransactionDate(this->dateToday);
        transaction_now.setDescription(customer.CustomerName,"INTEREST CREDITED TO CUSTOMER ACCOUNT",((dailyInterest) / 100) * (customer.moneyDeposit));
        bank.transactions.push_back(transaction_now);
        customer.transactions.push_back(transaction_now);

    }
}

string Administrator::get_Employee_Name(Bank& bank)
{
    return bank.employeeArray.back().get_Employee_Name();
}

string Administrator::get_Date()
{
    return this->dateToday;
}

float Administrator::get_annualSavingInterest()
{
    return (this->annualSavingInterest);
};

float Administrator::get_overdraftCharge()
{
    return (this->overdraftCharge);
};

void Customer::view_Transaction()
{
    int numberOfTransactions=this->transactions.size();
    for(int i=0; i<numberOfTransactions; i++)
    {
        cout<<this->transactions[i].getDescription()<<endl;
    }
};

void Customer::deposit_Money_Function(Bank& bank,ofstream& MyFile,ofstream& BankFile)
{
    int moneyAdded;
    cout << "ENTER THE AMOUNT OF MONEY TO DEPOSIT: ";
    cin >> moneyAdded;
    this -> moneyDeposit += moneyAdded;
    MyFile<<this->dateToday<<" - "<<this->CustomerName<<" : DEPOSITED "<<to_string(moneyAdded)<<endl;
    Transaction transaction_now_1;
    transaction_now_1.setTransactionDate(this->dateToday);
    transaction_now_1.setDescription(this->CustomerName,"MONEY CREDITED TO CUSTOMER ACCOUNT",moneyAdded);
    this->transactions.push_back(transaction_now_1);

    if (this -> moneyDeposit >= 0)
    {
        bank.bankBalance+=overDraftNow;
        BankFile<<this->dateToday<<" - "<<this->CustomerName<<" : OVERDRAFT CREDITED TO BANK ACCOUNT : "<<to_string(overDraftNow)<<" Rs."<<endl;
        BankFile<<this->dateToday<<" - "<<"BANK BALANCE : "<<bank.bankBalance<<endl;
        Transaction transaction_now_2;
        transaction_now_2.setTransactionDate(this->dateToday);
        transaction_now_2.setDescription(this->CustomerName,"OVERDRAFT CREDITED TO BANK ACCOUNT",overDraftNow);
        bank.transactions.push_back(transaction_now_2);
        this -> overDraftNow = 0;
    }
    else
    {
        bank.bankBalance+=moneyAdded;
        BankFile<<this->dateToday<<" - "<<this->CustomerName<<" : OVERDRAFT CREDITED TO BANK ACCOUNT : "<<to_string(moneyAdded)<<" Rs."<<endl;
        BankFile<<this->dateToday<<" - "<<"BANK BALANCE : "<<bank.bankBalance<<endl;
        this -> overDraftNow -= moneyAdded;
        Transaction transaction_now_2;
        transaction_now_2.setTransactionDate(this->dateToday);
        transaction_now_2.setDescription(this->CustomerName,"OVERDRAFT CREDITED TO BANK ACCOUNT",moneyAdded);
        bank.transactions.push_back(transaction_now_2);
    }
    for(int i=0; i<=bank.customerNameArray.size(); i++)
    {
        if(this->CustomerName==bank.customerNameArray[i])
        {
            bank.customerArray[i].moneyDeposit=this->moneyDeposit;
            bank.customerArray[i].overDraftNow=this->overDraftNow;
        }
    }
}

void Customer::set_Date(Administrator & Admin)
{
    this -> dateToday = Admin.get_Date();
};

void Customer::withdraw_Money(Bank& bank,ofstream& MyFile,ofstream& BankFile)
{
    int moneyToWithdraw;
    cout << "ENTER THE AMOUNT OF MONEY TO WITHDRAW: ";
    cin >> moneyToWithdraw;
    if (this -> isCurrentAccount == false && moneyToWithdraw > this -> moneyDeposit)
    {
        cout << "SORRY INSUFFICIENT BALANCE" << endl;
    }
    else if (this -> isCurrentAccount == false && moneyToWithdraw <= this -> moneyDeposit)
    {
        this -> moneyDeposit -= moneyToWithdraw;
        cout << "SUCESSFULL WITHDRAWAL" << endl;
        MyFile<<this->dateToday<<" - "<<this->CustomerName<<" : WITHDRAWN "<<to_string(moneyToWithdraw)<<endl;
        Transaction transaction_now_1;
        transaction_now_1.setTransactionDate(this->dateToday);
        transaction_now_1.setDescription(this->CustomerName,"MONEY WITHDRAWN FROM CUSTOMER ACCOUNT",moneyToWithdraw);
        this->transactions.push_back(transaction_now_1);
    }
    else if (this -> isCurrentAccount == true && moneyToWithdraw > this -> moneyDeposit)
    {
        if (((this -> moneyDeposit - moneyToWithdraw) * (-1)) <= this -> overdraftMaximum && ((this -> moneyDeposit - moneyToWithdraw) * (-1)) <= bank.bankBalance )
        {
            this -> overDraftNow = moneyToWithdraw - (this -> moneyDeposit);
            bank.bankBalance-=this->overDraftNow;
            this -> moneyDeposit -= moneyToWithdraw;
            cout << "SUCESSFULL WITHDRAWAL" << endl;
            BankFile<<this->dateToday<<" - "<<this->CustomerName<<" : OVERDRAFT DEBITED TO BANK ACCOUNT : "<<to_string(this->overDraftNow)<<" Rs."<<endl;
            BankFile<<this->dateToday<<" - "<<"BANK BALANCE : "<<bank.bankBalance<<endl;
            MyFile<<this->dateToday<<" - "<<this->CustomerName<<" : WITHDRAWN "<<to_string(moneyToWithdraw)<<endl;
            Transaction transaction_now_2;
            Transaction transaction_now_3;
            transaction_now_2.setTransactionDate(this->dateToday);
            transaction_now_3.setTransactionDate(this->dateToday);
            transaction_now_2.setDescription(this->CustomerName,"MONEY WITHDRAWN FROM CUSTOMER ACCOUNT",moneyToWithdraw);
            transaction_now_3.setDescription(this->CustomerName,"OVERDRAFT DEBITED TO BANK ACCOUNT",this->overDraftNow);
            this->transactions.push_back(transaction_now_2);
            bank.transactions.push_back(transaction_now_3);
        }
        else if(((this -> moneyDeposit - moneyToWithdraw) * (-1)) <= this -> overdraftMaximum && ((this -> moneyDeposit - moneyToWithdraw) * (-1)) > bank.bankBalance)
        {
            cout << "CAN'T WITHDRAW AMOUNT GREATER THAN BALANCE IN THE BANK.";
        }
        else
        {
            cout << "CAN'T WITHDRAW AMOUNT GREATER THAN MAXIMUM OVERDRAFT.";
        }
    }
    else if (this -> isCurrentAccount == true && moneyToWithdraw <= this -> moneyDeposit)
    {
        this -> moneyDeposit -= moneyToWithdraw;
        cout << "SUCESSFULL WITHDRAWAL" << endl;
        MyFile<<this->dateToday<<" - "<<this->CustomerName<<" : WITHDRAWN "<<to_string(moneyToWithdraw)<<endl;
        Transaction transaction_now_4;
        transaction_now_4.setTransactionDate(this->dateToday);
        transaction_now_4.setDescription(this->CustomerName,"MONEY WITHDRAWN FROM CUSTOMER ACCOUNT",moneyToWithdraw);
        this->transactions.push_back(transaction_now_4);
    }
    for(int i=0; i<=bank.customerNameArray.size(); i++)
    {
        if(this->CustomerName==bank.customerNameArray[i])
        {
            bank.customerArray[i].moneyDeposit=this->moneyDeposit;
            bank.customerArray[i].overDraftNow=this->overDraftNow;
        }
    }

};

