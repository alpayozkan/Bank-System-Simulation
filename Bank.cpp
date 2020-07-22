#include "Bank.h"

int isInUserList(Account *users, int user_count, const Account &rhs){
    if(users!= nullptr and user_count!=0){
        for (int i = 0; i < user_count; ++i) {
            if (users[i] == rhs)
                return i; // found index
        }
    }
    return -1; // not found
}

double totalBalanceBank(Account *users, int user_count)
{
    double sum=0;

    for (int i = 0; i < user_count; ++i) {
        sum += users[i].balance();
    }

    return  sum;
}

bool isEligibleAccount(Account &acc)
{
    struct tm timeInfo{};
    timeInfo.tm_year = 2019 - 1900;
    timeInfo.tm_mon = 0;
    timeInfo.tm_mday = 1;

    double totalBalance=0;
    double prevBalance=0,currBalance;

    time_t prevTime = mktime(&timeInfo), currTime;

    for (int i = 0; i < 11; ++i) { // 0-10 first 11 months
        timeInfo.tm_mon += 1;
        currTime = mktime(&timeInfo);
        currBalance = acc.balance(prevTime, currTime);

        totalBalance += currBalance; // sum first 11 months

        if(currBalance<0 and prevBalance<0)
            return false;

        prevBalance=currBalance;
        prevTime = currTime;
    }
    double lastBalance=acc.balance()-totalBalance; // totalBalance - balance of 11 months
    if(currBalance<0 and lastBalance<0)
        return false;

    return true;
}

Bank::Bank() {
    this->_bank_name = "not_defined";
    this->_users = nullptr;
    this->_user_count = 0;
}

Bank::Bank(std::string bank_name, Account *const users, int user_count) {
    this->_bank_name = bank_name; // move ? or pass by val
    this->_user_count = user_count;

    this->_users= new Account[this->_user_count];

    for (int i = 0; i < this->_user_count; ++i) {
        this->_users[i]=users[i];
    }
}

Bank::~Bank() {
    if(this->_users!= nullptr) {
        delete[] this->_users;
        this->_users= nullptr;
    }
}

Bank::Bank(const Bank &rhs) {
    this->_bank_name= rhs._bank_name;
    this->_user_count= rhs._user_count;

    this->_users = new Account[this->_user_count];

    for (int i = 0; i < this->_user_count; ++i) {
        this->_users[i] = rhs._users[i];
    }
}

Bank &Bank::operator+=(const Bank &rhs) {

    if(rhs._users== nullptr or rhs._user_count==0)
        return (*this);

    if(this->_users!= nullptr and this->_user_count!=0){
        int new_users = 0;
        for (int i = 0; i < rhs._user_count; ++i) {
            if (isInUserList(this->_users, this->_user_count, rhs._users[i]) == -1)
                new_users++;
        }

        if(new_users==0){ // if no new users -> just merge accounts
            for (int i = 0; i < rhs._user_count; ++i) {
                int index = isInUserList(this->_users, this->_user_count, rhs._users[i]);
                this->_users[index]+=rhs._users[i];
            }
            return (*this);
        }
        // if new_user>0
        Account *tmp = new Account[this->_user_count + new_users];
        for (int j = 0; j < this->_user_count; ++j) {
            tmp[j] = this->_users[j];
        }


        for (int k = 0, i = 0; k < rhs._user_count and i < new_users; ++k) {
            int index = isInUserList(this->_users, this->_user_count, rhs._users[k]);

            if (index != -1) {
                tmp[index] += rhs._users[k];
            } else {
                tmp[i + this->_user_count] = rhs._users[k];
                i++;
            }
        }

        delete[] this->_users;


        this->_users = tmp;
        tmp = nullptr;
        this->_user_count += new_users;

    }else{
        this->_user_count = rhs._user_count;
        this->_users = new Account[this->_user_count];

        for (int i = 0; i < this->_user_count; ++i) {
            this->_users[i] = rhs._users[i];
        }
    }
    return (*this);
}

Bank &Bank::operator+=(const Account &new_acc) {

    if(this->_users!= nullptr and this->_user_count!=0){

        int index = isInUserList(this->_users, this->_user_count, new_acc);
        if(index == -1){ // not found then append

            Account *tmp = new Account[this->_user_count + 1];
            // copy from this
            for (int j = 0; j < this->_user_count; ++j) {
                tmp[j] = this->_users[j];
            }
            // append new one
            tmp[this->_user_count] = new_acc;
            // free old users
            delete[] this->_users;
            //update this->users
            this->_users = tmp;
            // cut the connection for safety
            tmp = nullptr;
            // +1 count new
            this->_user_count += 1;
        }else{
            this->_users[index] +=new_acc;
        }

    }else{
        this->_user_count = 1;
        this->_users = new Account[1];
        this->_users[0] = new_acc;
    }

    return (*this);
}

Account &Bank::operator[](uint account_id) {

    if(this->_users!= nullptr and this->_user_count!=0){
        int index = 0;

        for (int i = 0; i < this->_user_count; ++i) {
            if (this->_users[i] == account_id) {
                index = i;
                break;
            }
        }

        return this->_users[index];

    }else{
        Account *a = new Account; // sacmalik durumu
        return *a;
    }
}

std::ostream &operator<<(std::ostream &os, const Bank &bank) {

    os << bank._bank_name;
    os << "\t";

    int countEligible=0;
    for (int i = 0; i < bank._user_count; ++i) {
        if(isEligibleAccount(bank._users[i]))
            countEligible++;
    }
    os << countEligible;
    os << "\t";

    os << totalBalanceBank(bank._users, bank._user_count);
    os << std::endl;

    return os;
}




