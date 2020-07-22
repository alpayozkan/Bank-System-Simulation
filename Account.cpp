#include "Account.h"

void sortTransactions(Transaction *&act, int tmp_act) // can be changed to qsort or mergesort
{

    if(act==nullptr or tmp_act<2)
        return;

    Transaction tmp; // for swaping tmp variable

    for (size_t j = 0; j < tmp_act; j++) // bubblesort for each month i // tmp_act 0 ise loop girilmez no problem
        {
            size_t min = j;

            for (size_t k = j+1; k < tmp_act; k++)
            {
                if(act[k] < act[min])
                    min=k;
            }
            tmp = act[j];
            act[j] = act[min];
            act[min] = tmp;
        }

}



Account::Account()
{
    _id=-1;
    _activity=nullptr;
    _monthly_activity_frequency=nullptr;
}

Account::Account(int id, Transaction** const activity, int* monthly_activity_frequency)
{
    this->_id = id;

    if(activity!= nullptr and monthly_activity_frequency!= nullptr)
    {
        this->_activity = new Transaction*[12];
        this->_monthly_activity_frequency = new int[12];

        for (size_t i = 0; i < 12; i++)
        {
            int tmp_act = monthly_activity_frequency[i];

            this->_monthly_activity_frequency[i] = tmp_act;

            if(tmp_act!=0) // non zero elems -> allocate
                this->_activity[i] = new Transaction[tmp_act];
            else // 0 elem -> no allocation
            {
                this->_activity[i] = nullptr;
                continue;
            }

            for (size_t j = 0; j < tmp_act; j++) // copy for each month
            {
                this->_activity[i][j]=activity[i][j]; // tmp_act 0 ise loop a girmicek zaten
            }

            sortTransactions(this->_activity[i], this->_monthly_activity_frequency[i]); // sort each month
        }

    } else{
        this->_activity = nullptr;
        this->_monthly_activity_frequency = nullptr;
    }

}

Account::~Account()
{
    if(this->_activity!=nullptr and this->_monthly_activity_frequency!=nullptr) // nullptr ise delete lenmez
    {
        for (size_t i = 0; i < 12; i++)
        {
            if(this->_activity[i] != nullptr) {
                delete[] this->_activity[i];
                this->_activity[i]= nullptr;
            }
        }

        delete[] this->_activity;
        this->_activity= nullptr;

        delete[] this->_monthly_activity_frequency;
        this->_monthly_activity_frequency= nullptr;
    }
    
}

Account::Account(const Account& rhs)
{
    this->_id = rhs._id;

    if(rhs._activity!= nullptr and rhs._monthly_activity_frequency!= nullptr)
    {
        this->_monthly_activity_frequency = new int[12];
        this->_activity = new Transaction *[12];

        for (size_t i = 0; i < 12; i++) {
            int tmp_act = rhs._monthly_activity_frequency[i];

            this->_monthly_activity_frequency[i] = tmp_act;

            if (tmp_act != 0) // non zero elems -> allocate
                this->_activity[i] = new Transaction[tmp_act];
            else // 0 elem -> no allocation
            {
                this->_activity[i] = nullptr;
                continue;
            }

            for (size_t j = 0; j < tmp_act; j++) {
                this->_activity[i][j] = rhs._activity[i][j];
            }

            sortTransactions(this->_activity[i], this->_monthly_activity_frequency[i]);
        }
    }
    else{
        this->_activity = nullptr;
        this->_monthly_activity_frequency = nullptr;
    }
}


Account::Account(const Account& rhs, time_t start_date, time_t end_date)
{
    // check if activity and monthly are nullptr or empty otherwise first free them
    this->_id = rhs._id;

    if(rhs._activity!= nullptr and rhs._monthly_activity_frequency!= nullptr) {

        this->_monthly_activity_frequency = new int[12]; // num of acts in a month allocated
        this->_activity = new Transaction *[12]; // allocate months transaction*

        for (size_t i = 0; i < 12; i++) // intz zero before counting
        {
            this->_monthly_activity_frequency[i] = 0;
        }

        for (size_t i = 0; i < 12; i++) {

            int tmp_act = rhs._monthly_activity_frequency[i];

            for (size_t j = 0; j < tmp_act; j++) {
                if (rhs._activity[i][j] < end_date and rhs._activity[i][j] > start_date)
                    this->_monthly_activity_frequency[i]++; // count num of spaces for allocations
            }

            if (this->_monthly_activity_frequency[i] != 0) // non zero elems -> allocate
                this->_activity[i] = new Transaction[this->_monthly_activity_frequency[i]]; //allocate enough space
            else // 0 elem -> no allocation
            {
                this->_activity[i] = nullptr;
                continue;
            }

            for (size_t j = 0, k = 0; j < tmp_act; j++) // copy transactions
            {
                if (rhs._activity[i][j] < end_date and rhs._activity[i][j] > start_date) {
                    this->_activity[i][k] = rhs._activity[i][j];
                    k++;
                }
            }

            sortTransactions(this->_activity[i], this->_monthly_activity_frequency[i]); // tmp_act olmaz cunku o rhs in bizimki daha az : burdan cikan bug dan ders su ki adam gibi isimlendir variablelari kafami frack etti bulmak
        }
    } else{
        this->_activity = nullptr;
        this->_monthly_activity_frequency = nullptr;
    }
}

Account::Account(Account&& rhs)
{
    this->_id = rhs._id;

    if(rhs._activity != nullptr and rhs._monthly_activity_frequency != nullptr){
        this->_monthly_activity_frequency = rhs._monthly_activity_frequency;
        this->_activity = rhs._activity;

        for (size_t i = 0; i < 12; i++) {
            if(this->_activity[i] != nullptr)
                sortTransactions(this->_activity[i], this->_monthly_activity_frequency[i]);
        }

        // move constructor semantics -> cut the connection from temporary rvalue which will be destroyed
        rhs._monthly_activity_frequency = nullptr;
        rhs._activity = nullptr;

    } else{
        this->_activity = nullptr;
        this->_monthly_activity_frequency = nullptr;
    }
}


Account& Account::operator=(Account&& rhs)
{
    // clean previous data if there were any
    if(this->_activity!=nullptr and this->_monthly_activity_frequency!=nullptr)
    {
        for (size_t i = 0; i < 12; i++)
        {   if(this->_activity[i] != nullptr) {
                delete[] this->_activity[i];
                this->_activity[i]= nullptr;
            }
        }

        delete[] this->_activity;
        this->_activity= nullptr;

        delete[] this->_monthly_activity_frequency;
        this->_monthly_activity_frequency= nullptr;
    }
    
    // move resources

    this->_id = rhs._id;


    if(rhs._activity!=nullptr and rhs._monthly_activity_frequency!= nullptr){

        this->_monthly_activity_frequency = rhs._monthly_activity_frequency;
        this->_activity = rhs._activity;

        //sort transactions in each month
        for (size_t i = 0; i < 12; i++) {
            if(this->_activity[i]!= nullptr)
                sortTransactions(this->_activity[i], this->_monthly_activity_frequency[i]);
        }

        // cut the connection betw &&rvalue and the object : &&rvalue is a temp object and allocated memory will be deallocated after the procedure
        rhs._monthly_activity_frequency = nullptr;
        rhs._activity = nullptr;

    }else{
        this->_activity = nullptr;
        this->_monthly_activity_frequency = nullptr;
    }

    return *this;
}

Account& Account::operator=(const Account& rhs)
{
    // clean previous data if there were any
    if(this->_activity!=nullptr and this->_monthly_activity_frequency!=nullptr)
    {
        for (size_t i = 0; i < 12; i++)
        {   if(this->_activity[i] != nullptr) {
                delete[] this->_activity[i];
                this->_activity[i]= nullptr;
            }
        }

        delete[] this->_activity;
        this->_activity= nullptr;

        delete[] this->_monthly_activity_frequency;
        this->_monthly_activity_frequency= nullptr;
    }

    this->_id=rhs._id;

    if(rhs._activity != nullptr and rhs._monthly_activity_frequency != nullptr){

        this->_monthly_activity_frequency = new int[12];
        this->_activity = new Transaction *[12];

        for (size_t i = 0; i < 12; i++) {

            int tmp_act = rhs._monthly_activity_frequency[i];

            this->_monthly_activity_frequency[i] = tmp_act;

            if (tmp_act != 0) // non zero elems -> allocate
                this->_activity[i] = new Transaction[tmp_act];
            else // 0 elem -> no allocation
            {
                this->_activity[i] = nullptr;
                continue;
            }


            for (size_t j = 0; j < tmp_act; j++) {
                this->_activity[i][j] = rhs._activity[i][j];
            }

            sortTransactions(this->_activity[i], this->_monthly_activity_frequency[i]);
        }
    } else{
        this->_activity = nullptr;
        this->_monthly_activity_frequency = nullptr;
    }

    return *this;
}

bool Account::operator==(const Account& rhs) const
{
    return this->_id==rhs._id;
}

bool Account::operator==(int id) const
{
    return this->_id==id;
}

Account& Account::operator+=(const Account& rhs)
{
    if(rhs._activity != nullptr and rhs._monthly_activity_frequency != nullptr){// create a tmp account variable merge this and rhs and at the end assign tmp to this

        Account tmp;
        tmp._id = this->_id;
        tmp._monthly_activity_frequency = new int[12];
        tmp._activity = new Transaction *[12];

        // merge act size of tmp from this + rhs
        for (size_t i = 0; i < 12; i++) {
            tmp._monthly_activity_frequency[i] = this->_monthly_activity_frequency[i] + rhs._monthly_activity_frequency[i];
        }

        for (size_t i = 0; i < 12; i++) {
            int tmp_act = tmp._monthly_activity_frequency[i];
            int tmp_act_this = this->_monthly_activity_frequency[i];

            if (tmp_act != 0)
                tmp._activity[i] = new Transaction[tmp_act];
            else {
                tmp._activity[i] = nullptr;
                continue;
            }

            // copy from this
            for (size_t j = 0; j < tmp_act_this; j++) {
                tmp._activity[i][j] = this->_activity[i][j];
            }
            // copy from rhs
            for (size_t j = tmp_act_this; j < tmp_act; j++) {
                tmp._activity[i][j] = rhs._activity[i][j-tmp_act_this];
            }
            // sort tmp
            sortTransactions(tmp._activity[i], tmp._monthly_activity_frequency[i]);
        }

        (*this) = tmp;

    } else{
        this->_activity = nullptr;
        this->_monthly_activity_frequency = nullptr;
    }

    return  *this;
}

double Account::balance()
{
    double sum = 0;

    if(this->_activity!= nullptr and this->_monthly_activity_frequency!= nullptr){
        for (size_t i = 0; i < 12; i++) {

            if(this->_activity[i]!=nullptr){

                int tmp_act = this->_monthly_activity_frequency[i];

                for (size_t j = 0; j < tmp_act; j++) {
                    sum = this->_activity[i][j] + sum;
                }
            }

        }
    }
    return sum;
}

double Account::balance(time_t end_date)
{
    double sum = 0;

    if(this->_activity!= nullptr and this->_monthly_activity_frequency!= nullptr){
        for (size_t i = 0; i < 12; i++) {

            if(this->_activity[i]!=nullptr){

                int tmp_act = this->_monthly_activity_frequency[i];

                for (size_t j = 0; j < tmp_act; j++) {
                    if(this->_activity[i][j] < end_date)
                        sum = (this->_activity[i][j])+sum;
                }
            }

        }
    }

    return sum;   
}

double Account::balance(time_t start_date, time_t end_date)
{
    double sum = 0;

    if(this->_activity!= nullptr and this->_monthly_activity_frequency!= nullptr){
        for (size_t i = 0; i < 12; i++) {

            if(this->_activity[i]!=nullptr){

                int tmp_act = this->_monthly_activity_frequency[i];

                for (size_t j = 0; j < tmp_act; j++) {
                    if(this->_activity[i][j] < end_date and this->_activity[i][j] > start_date)
                        sum = (this->_activity[i][j])+sum;
                }
            }

        }
    }

    return sum;
}
std::ostream& operator<<(std::ostream& os, const Account& account)
{
    
    
    if(account._monthly_activity_frequency != nullptr and account._activity != nullptr)
    {
        os << account._id << std::endl;

        for (size_t i = 0; i < 12; i++)
        {
            int tmp_act = account._monthly_activity_frequency[i];

            for (size_t j = 0; j < tmp_act; j++)
            {
                os << account._activity[i][j];
            }

        }    
    }
    else
    {
        os << -1 << std::endl;
    }
    
    return os;
}