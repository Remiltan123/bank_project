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













int main()
{
    Bank bank;
    vector<string> employeeNameArray;
    vector<string> customerNameArray;

    ofstream BankFile("Bank.txt");
    ofstream CustomerFile("Customer_details.txt");
    fstream BankFile_handler;
    fstream CustomerFile_handler;

    BankFile_handler.open("Bank.txt", ios:: in | ios::out);
    CustomerFile_handler.open("Customer_details.txt", ios:: in | ios::out);

    if (!BankFile_handler)
    {
        cout << "BANK FILE DID NOT OPEN!";
        exit(1);
    }

    if (!CustomerFile_handler)
    {
        cout << "CUSTOMER DETAILS FILE DID NOT OPEN!";
        exit(1);
    }

    int input=1,user_input_1, user_input_2,user_input_3;
    Administrator Admin;
    Admin.set_Date();
    string username, password;
    while(input!=0)
    {
        cout << "SELECT AN OPTION BELOW : " << endl;
        cout << "1. DEPOSIT AS AN OUTSIDE SOURCE." << endl;
        cout << "2. LOGIN." << endl;
        cin >> user_input_1;
        if (user_input_1 == 2)
        {
            int login_checker=0;
            while(login_checker!=1)
            {
                cout << "SELECT AN OPTION BELOW : " << endl;
                cout << "1. ENTER AS ADMINISTRATOR." << endl;
                cout << "2. ENTER AS CUSTOMER." << endl;
                cout << "3. ENTER AS EMPLOYEE." << endl;
                cout<<"4. BANK"<<endl;

                cin >> user_input_2;
                if (user_input_2 == 1)
                {
                    cout << "ENTER THE USER NAME : ";
                    cin >> username;
                    cout << endl;
                    password = getPassword();

                    while (username != "Admin" || password != "Password@1234")
                    {
                        cout << "INVALID CREDENTIALS." << endl;
                        cout << "ENTER THE USER NAME : ";
                        cin >> username;
                        cout << endl;
                        password = getPassword();
                    }
                    cout << "SUCCESSFUL LOGIN" << endl<<endl;
                    int checker_admin=0;
                    while(checker_admin!=1)
                    {
                        cout<<"\nDATE : "<<Admin.get_Date()<<endl;
                        float annualInterest=Admin.get_annualSavingInterest();
                        float overdraftCharge=Admin.get_overdraftCharge();
                        cout<<"ANNUAL INTEREST RATE IS "<<annualInterest<<endl;
                        cout<<"OVERDRAFT CHARGE IS "<<overdraftCharge<<endl;
                        cout<<"BANK BALANCE NOW :  "<<bank.bankBalance<<endl<<endl;
                        // cout<<"Overdraft charge per day is " <<" % "<<end;
                        cout<<"1. CREATE AN EMPLOYEE"<<endl;
                        cout<<"2. INCREASE THE DATE"<<endl;
                        cout<<"3. CHANGE OVERDRAFT CHARGE"<<endl;
                        cout<<"4. CHANGE ANNUAL SAVINGS INTEREST"<<endl;
                        cout<<"5. LOGOUT"<<endl;
                        cout <<"SELECT AN OPTION ABOVE"<<endl;
                        cin>>user_input_3;
                        switch(user_input_3)
                        {
                        case 1:
                        {
                            Admin.create_Employee(bank);
                            Employee employee;
                            break;
                        }
                        case 2:
                        {
                            Admin.increase_Date();
                            if(bank.customerArray.size()>0)
                            {
                                for(int i=0; i<bank.customerArray.size(); i++)
                                {
                                    Admin.update_Customer_Interest(bank.customerArray[i],bank,BankFile);
                                    Admin.update_Customer_Overdraft(bank.customerArray[i]);
                                }
                            }
                            break;
                        }
                        case 3:
                        {
                            Admin.set_overdraftCharge();
                            break;
                        }
                        case 4:
                        {
                            Admin.set_annualSavingInterest();
                            break;
                        }
                        case 5:
                        {
                            input=0;
                            checker_admin=1;
                            break;
                        }
                        default:
                        {}
                        }
                    }
                }
                else if(user_input_2==4)
                {
                    login_checker=1;
                    input=1;
                    break;
                }
                else if(user_input_2==3)
                {
                    cout << "ENTER THE EMPLOYEE USER NAME : ";
                    cin >> username;
                    cout << endl;
                    password = getPassword();
                    employeeNameArray=bank.employeeNameArray;
                    Employee employeeNow;
                    for(int i=0; i<employeeNameArray.size(); i++)
                    {
                        if(username==employeeNameArray[i] && password =="Password@1234")
                        {
                            employeeNow=bank.employeeArray[i];
                            login_checker=0;
                            break;
                        }
                        login_checker=1;
                    }
                    if(login_checker==1)
                    {
                        cout<<"LOGIN UNSUCCESSFUL";
                        login_checker=0;
                    }
                    else
                    {
                        cout << "SUCCESSFUL LOGIN" << endl<<endl;
                        int checker_employee=0;
                        while(checker_employee!=1)
                        {
                            cout<<"\nDATE : "<<Admin.get_Date()<<endl;
                            float annualInterest=Admin.get_annualSavingInterest();
                            float overdraftCharge=Admin.get_overdraftCharge();
                            cout<<"ANNUAL INTEREST RATE IS "<<annualInterest<<endl;
                            cout<<"OVERDRAFT CHARGE IS "<<overdraftCharge<<endl;
                            cout<<"1. CREATE A CUSTOMER ACCOUNT"<<endl;
                            cout<<"2. CLOSE THE CUSTOMER ACCOUNT"<<endl;
                            cout<<"3. DEPOSIT MONEY IN ACCOUNT"<<endl;
                            cout<<"4. WITHDRAW MONEY FROM ACCOUNT"<<endl;
                            cout<<"5. VIEW TRANSACTIONS"<<endl;
                            cout<<"6. LOGOUT"<<endl;
                            cout <<"SELECT AN OPTION ABOVE"<<endl;
                            cin>>user_input_3;
                            switch(user_input_3)
                            {
                            case 1:
                            {
                                Customer customerCreated;
                                customerCreated.set_Date(Admin);
                                employeeNow.create_Customer(customerCreated,bank,CustomerFile);
                                break;
                            }
                            case 2:
                            {
                                string customerName;
                                cout<<"ENTER THE NAME OF THE CUSTOMER: ";
                                cin>>customerName;
                                for(int i=0; i<bank.customerArray.size(); i++)
                                {
                                    if(customerName==bank.customerArray[i].CustomerName)
                                    {
                                        bank.customerArray[i].set_Date(Admin);
                                        employeeNow.close_Customer_Account(bank.customerArray[i],CustomerFile);
                                        break;
                                    }
                                }
                                break;
                            }
                            case 3:
                            {
                                string customerName;
                                cout<<"ENTER THE NAME OF THE CUSTOMER: ";
                                cin>>customerName;
                                for(int i=0; i<bank.customerArray.size(); i++)
                                {
                                    if(customerName==bank.customerArray[i].CustomerName)
                                    {
                                        bank.customerArray[i].set_Date(Admin);
                                        employeeNow.deposit_Money(bank.customerArray[i],bank,CustomerFile,BankFile);
                                        break;
                                    }
                                }
                                break;
                            }
                            case 4:
                            {
                                string customerName;
                                cout<<"ENTER THE NAME OF THE CUSTOMER: ";
                                cin>>customerName;
                                for(int i=0; i<bank.customerArray.size(); i++)
                                {
                                    if(customerName==bank.customerArray[i].CustomerName)
                                    {
                                        bank.customerArray[i].set_Date(Admin);
                                        employeeNow.withdraw_Money(bank.customerArray[i],bank,CustomerFile,BankFile);
                                        break;
                                    }
                                }
                                break;
                            }
                            case 5:
                            {
                                employeeNow.view_Transactions(bank);
                                break;
                            }
                            case 6:
                            {
                                input=0;
                                checker_employee=1;
                                break;
                            }
                            default:
                            {}
                            }
                        }
                    }


                }
                else if(user_input_2==2)
                {
                    int customerArrayElement;
                    cout << "ENTER THE CUSTOMER USER NAME : ";
                    cin >> username;
                    cout << endl;
                    password = getPassword();
                    customerNameArray=bank.customerNameArray;
                    Customer customer;
                    for(int i=0; i<customerNameArray.size(); i++)
                    {
                        cout<<customerNameArray[i]<<endl;
                    }
                    for(int i=0; i<customerNameArray.size(); i++)
                    {
                        if(username==customerNameArray[i] && password =="Password@1234")
                        {
                            customer=bank.customerArray[i];
                            customerArrayElement=i;
                            login_checker=0;
                            break;
                        }
                        login_checker=1;
                    }
                    if(login_checker==1)
                    {
                        cout<<"LOGIN UNSUCCESSFUL";
                        login_checker=0;
                    }
                    else
                    {
                        cout << "SUCCESSFUL LOGIN" << endl<<endl;
                        int checker_customer=0;
                        while(checker_customer!=1)
                        {
                            cout<<"\nDATE : "<<Admin.get_Date()<<endl;
                            float annualInterest=Admin.get_annualSavingInterest();
                            float overdraftCharge=Admin.get_overdraftCharge();
                            cout<<"ANNUAL INTEREST RATE IS "<<annualInterest<<endl;
                            cout<<"OVERDRAFT CHARGE IS "<<overdraftCharge<<endl<<endl;
                            cout<<"ACCOUNT NAME : "<<customer.CustomerName<<endl;
                            cout<<"ACCOUNT TYPE : "<<customer.accountType<<endl;
                            cout<<"ACCOUNT CONTACT DETAILS : "<<customer.customerContact<<endl<<endl;
                            cout<<"ACCOUNT BALANCE : "<<customer.moneyDeposit<<endl<<endl;
                            cout<<"ACCOUNT OVERDRAFT : "<<customer.overDraftNow<<endl<<endl;
                            cout<<"1. DEPOSIT MONEY IN ACCOUNT"<<endl;
                            cout<<"2. WITHDRAW MONEY FROM ACCOUNT"<<endl;
                            cout<<"3. VIEW TRANSACTIONS"<<endl;
                            cout<<"4. LOGOUT"<<endl;
                            cout <<"SELECT AN OPTION ABOVE"<<endl;
                            cin>>user_input_3;
                            switch(user_input_3)
                            {
                            case 1:
                            {
                                if(customer.isClosed==false)
                                {
                                    customer.set_Date(Admin);
                                    customer.deposit_Money_Function(bank,CustomerFile,BankFile);
                                    bank.customerArray[customerArrayElement]=customer;
                                }
                                else
                                {
                                    cout<<"ACCOUNT CLOSED ALREADY."<<endl;
                                }
                                break;
                            }
                            case 2:
                            {
                                if(customer.isClosed==false)
                                {
                                    customer.set_Date(Admin);
                                    customer.withdraw_Money(bank,CustomerFile,BankFile);
                                    bank.customerArray[customerArrayElement]=customer;
                                }
                                else
                                {
                                    cout<<"ACCOUNT CLOSED ALREADY."<<endl;
                                }
                                break;
                            }
                            case 3:
                            {
                                customer.view_Transaction();
                                break;
                            }
                            case 4:
                            {
                                input=0;
                                checker_customer=1;
                                break;
                            }
                            default:
                            {}
                            }
                        }
                    }
                }

            }
        }
        
    }
    return 0;
}
