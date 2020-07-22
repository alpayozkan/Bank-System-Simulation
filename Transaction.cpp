#include "Transaction.h"


Transaction::Transaction()
{
    this->_amount = -1;
    this->_date = -1;
}

Transaction::Transaction(double amount, time_t date)
{
    this->_amount = amount;
    this->_date = date;
}

Transaction::Transaction(const Transaction& rhs)
{
    this->_amount = rhs._amount;
    this->_date = rhs._date;
}

bool Transaction::operator<(const Transaction& rhs) const
{
    return this->_date < rhs._date;
}

bool Transaction::operator>(const Transaction& rhs) const
{
    return this->_date > rhs._date;
}



bool Transaction::operator<(const time_t date) const
{
    return this->_date < date;
}

bool Transaction::operator>(const time_t date) const
{
    return this->_date > date;
}


double Transaction::operator+(const Transaction& rhs)
{
    return this->_amount + rhs._amount;
}

double Transaction::operator+(const double add)
{
    return this->_amount + add;
}

Transaction& Transaction::operator=(const Transaction& rhs) // problem var
{
    this->_amount = rhs._amount;
    this->_date = rhs._date;


    return (*this);
}

std::ostream& operator<<(std::ostream& os, const Transaction& transaction) // friend function in scope u indicate edilmeli mi Transaction::operator<< diye?
{
    struct tm *tmp = localtime(&transaction._date);

    os << transaction._amount;
    os << "\t-\t";

    os << tmp->tm_hour << ":" << tmp->tm_min << ":" << tmp->tm_sec << "-" << tmp->tm_mday << "/" << tmp->tm_mon+1 << "/" << 1900+tmp->tm_year << "\n";

    return os;    
} 
