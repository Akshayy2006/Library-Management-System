#include <bits/stdc++.h>
#include <ctime>
using namespace std;
#define ll long long

const int sec_in_day = 60*60*24; //no. of seconds in a day
const int u=3; //userid has u digits
const int b=4; //bookid has b digits

class transaction{
public:
    int bookid;
    string date;
    char action; // b=borrowed, r=returned
    transaction(){}
    transaction(string tran){
        action=tran[0];
        bookid = stoi(tran.substr(1,b));
        date = tran.substr(b+1);
    }
    string encode(){
        string tran;
        tran.push_back(action);
        tran.append(to_string(bookid));
        tran.append(date);
        return tran;
    }
};


// Function to convert std::tm to string
string dateToString(const std::tm& date) {
    stringstream ss;
    ss << put_time(&date, "%d-%m-%Y");
    return ss.str();
}

// Function to convert string back to std::tm
std::tm stringToDate(const string& dateStr) {
    std::tm date = {};
    stringstream ss(dateStr);
    ss >> get_time(&date, "%d-%m-%Y");
    if (ss.fail()) {
        cout<<"Failed to parse date string!";
    }
    return date;
}

// Function to calculate difference in days between two dates
int calculateDaysDifference(string& datestr1 , string& datestr2) {
    std::tm date1 = stringToDate(datestr1);
    std::tm date2 = stringToDate(datestr2);
    std::time_t time1 = std::mktime(const_cast<std::tm*>(&date1));
    std::time_t time2 = std::mktime(const_cast<std::tm*>(&date2));

    double diff_seconds = std::difftime(time2, time1);
    return static_cast<int>(diff_seconds / (60 * 60 * 24)); // Convert seconds to days
}


// 3 digit codes for users, 4 digit for books. no reason, arbit choice
// 000 or 0000 denote null
// all other records start from 1001 or 101
// passwords dont allow spaces, but wont throw error if you enter space
string available="available", borrowed="borrowed", reserved="reserved", nulldate="00-00-0000";

class book {
private:
    string status;
    int borrower_id;
    int reserved_for;
public:
    int bookID;
    string title;
    string author;
    string publisher;
    int year_of_publishing;
    string ISBN;
    book(){
        status = available;
        borrower_id = 0;
        reserved_for = 0;
    }
    book(string data){
        bookID = stoi(data.substr(0,b));
        int i=b+1;
        while(i<data.size()&& data[i]!='|'){title.push_back(data[i]);i++;}
        i++;
        while(i<data.size()&& data[i]!='|'){author.push_back(data[i]);i++;}
        i++;
        while(i<data.size()&& data[i]!='|'){publisher.push_back(data[i]);i++;}
        i++;
        year_of_publishing = stoi(data.substr(i,4));
        i+=5;
        while(i<data.size()&& data[i]!='|'){ISBN.push_back(data[i]);i++;}
        i++;
        while(i<data.size()&& data[i]!='|'){status.push_back(data[i]);i++;}
        i++;
        borrower_id = stoi(data.substr(i,u));
        i+=(u+1);
        reserved_for = stoi(data.substr(i,u));
    }
    bool borrow(int userid){
        if(status==reserved) {
            if(reserved_for==userid && borrower_id==0){
                status = borrowed;
                borrower_id = userid;
                reserved_for = 0;
                return true;
            }
            cout<<"Book not available. Try again later.\n";
            return false;
        }
        else if(status==borrowed) {
            cout<<"Book not available. Do you want to reserve the book? (Y=Yes, N=No) : ";
            string reply="N";
            cin>>reply;
            if(reply=="Y") {
                reserved_for=userid;
                status= reserved;
                cout<<"Book reserved. You can issue this book after the current recipient returns it.\n";
            }
            return false;
        }
        else if(status==available){
            status =borrowed;
            borrower_id = userid;
            return true;
        }
        return false;
    }
    bool return_book(void){
        if(status == borrowed) {status = available;}
        // if status is reserved, keep it reserved. but change borrower id to 0 to identify if the previous recipient has returned book, when the reserved_for tries to issue it next time.
        borrower_id = 0;
        return true;
    }
    string update_database(void){
        string data=to_string(bookID);
        data.push_back('|');
        data.append(title);
        data.push_back('|');
        data.append(author);
        data.push_back('|');
        data.append(publisher);
        data.push_back('|');
        data.append(to_string(year_of_publishing));
        data.push_back('|');
        data.append(ISBN);
        data.push_back('|');
        data.append(status);
        data.push_back('|');
        if(borrower_id==0) data.append("000");
        else data.append(to_string(borrower_id));
        data.push_back('|');
        if(reserved_for==0) data.append("000");
        else data.append(to_string(reserved_for));
        data.push_back('|');
        data.push_back('\n');
        return data;
    }
};    



class user{
public:
    int userID;
    bool verify_password(string pw){
        if(pw==password) return true;
        else return false;
    }
    bool set_password(string& pw, string& cnf){
        if(pw==cnf) {password=pw; return true;}
        else return false;
    }
    virtual bool borrow(vector<book>& bookdb, string today) = 0;
    virtual bool return_book(vector<book>& bookdb, string today) = 0;
    virtual void show_history() = 0;
protected:
    string password;
    char role;
    string username;
};


class student : public user {
private:
    int wallet;
    int current_borrowed[3];
    string dates_borrowed[3];
    const static int limit=3;
    vector<transaction> history;
public:
    student(int userid, string uname){
        userID=userid;
        username=uname;
        role='S';
        wallet=0;
        for(int i=0; i<limit; i++){
            current_borrowed[i]=0;
            dates_borrowed[i]=nulldate;
        }
    }
    student(string data){
        // limit=3;
        role=data[0];
        userID = stoi(data.substr(1,u));
        int i=u+2;
        while(i<data.size() && data[i]!='|'){password.push_back(data[i]); i++;}
        i++;
        while(i<data.size() && data[i]!='|'){username.push_back(data[i]); i++;}
        i++;
        string wlt;
        while(i<data.size() && data[i]!='|'){wlt.push_back(data[i]); i++;}
        i++;
        wallet=stoi(wlt);
        for(int j=0; j<limit; j++){
            current_borrowed[j] = stoi(data.substr(i,b));
            i+=(b+1);
        }
        for(int j=0; j<limit; j++){
            while(i<data.size() && data[i]!='|'){dates_borrowed[j].push_back(data[i]); i++;}
            i++;
        }
        string tran;
        while(data[i]!='~'){
            if(data[i]=='|') {history.push_back(transaction (tran)); tran.clear();}
            else tran.push_back(data[i]);
            i++;
        }
    }
    string update_database(void){
        string data;
        data.push_back(role);
        data.append(to_string(userID));
        data.push_back('|');
        data.append(password);
        data.push_back('|');
        data.append(username);
        data.push_back('|');
        data.append(to_string(wallet));
        data.push_back('|');
        for(int j=0; j<limit; j++){
            if(current_borrowed[j]==0) data.append("0000");
            else data.append(to_string(current_borrowed[j]));
            data.push_back('|');
        }
        for(int j=0; j<limit; j++){data.append(dates_borrowed[j]); data.push_back('|');}
        for(auto tr:history) {data.append(tr.encode()); data.push_back('|');}
        data.push_back('~');
        data.push_back('\n');
        return data;
    }
    int calculate_overdue(string today){
        int dues=0;
        for(int i=0; i<limit; i++){
            if(current_borrowed[i]!=0){
                int tmp=(calculateDaysDifference(dates_borrowed[i], today)-15);
                if(tmp>0) dues+=tmp;
            }
        }
        dues *= 10;
        return dues;
    }
    bool borrow(vector<book>& bookdb, string today){
        // check if 3 books issued
        int freeslot=0;
        for(; freeslot<limit; freeslot++) {if(current_borrowed[freeslot]==0) break;}
        if(freeslot==3) {cout<<"Borrow limit exceeded. Please return a borrowed book and try again."; return false;}

        // check for overdues
        int dues=calculate_overdue(today)-wallet;
        if(dues>0){
            cout<<"Pending dues. Books can't be issued if there are existing dues. Clear dues and try again.";
            return false;
        }

        cout<<"Enter Book ID : ";
        int bookid;
        cin>>bookid;
        if(bookid==0) {cout<<"Invalid book ID!\n"; return false;}
        int i=0;
        for(; i<bookdb.size(); i++){
            if((bookdb[i].bookID)==bookid) break;
        }
        if(i==bookdb.size()) {cout<<"Book not found!"; return false;}

        if(bookdb[i].borrow(userID)){
            current_borrowed[freeslot]=bookid;
            dates_borrowed[freeslot]=today;
            transaction tr;
            tr.action='b';
            tr.bookid=bookid;
            tr.date=today;
            history.push_back(tr);
            cout<<"Book borrowed successfully.\n";
            return true;
        } else {
            return false;
        }
    }

    bool return_book(vector<book>& bookdb, string today){
        cout<<"Enter Book ID : ";
        int bookid;
        cin>>bookid;
        if(bookid==0) {cout<<"Invalid book ID!\n"; return false;}
        int reqslot=-1;
        for(int i=0; i<limit; i++){
            if(current_borrowed[i]==bookid) {reqslot=i; break;}
        }
        if(reqslot==-1) {cout<<"Book not borrowed by user."; return false;}

        int i=0;
        for(; i<bookdb.size(); i++){
            if((bookdb[i].bookID)==bookid) break;
        }
        if(i==bookdb.size()) {cout<<"Book not found!"; return false;}
        bookdb[i].return_book();
        current_borrowed[reqslot]=0;
        int diff = ((calculateDaysDifference(dates_borrowed[reqslot], today)-15));
        if(diff>0) {
            cout<<"Book has been returned after overdue period. Total overdue period (in days) : "<<diff<<endl;
            diff*=10;
            cout<<"An amount of Rs."<<diff<<" has been added to your existing dues.\n";
            cout<<"Note that if you recently cleared some dues, then you would only have to pay the excess amount that has accumulated from that day.\n";
            cout<<"It is necessary to clear all dues before you borrow any more books. Dues can be cleared by going to View and Clear Dues option from main menu.\n";
            wallet-=diff;
        }
        dates_borrowed[reqslot]=nulldate;
        transaction tr;
        tr.action='r';
        tr.bookid=bookid;
        tr.date=today;
        history.push_back(tr);
        cout<<"Book returned successfully.\n";
        return true;
    }

    void check_dues_wallet(string today){
        int dues = calculate_overdue(today)-wallet;
        if(dues>0) {
            cout<<"Current pending dues = Rs."<<dues<<"\n";
            cout<<"Note that you cannot borrow books if you have pending dues.\nDo you want to clear your dues? (Y=Yes, N=No) : ";
            char reply;
            cin>>reply;
            if(reply == 'Y'){
                cout<<"Do you confirm a payment of Rs."<<dues<<" to clear your dues? (Y=Yes, N=No) : ";
                reply='N';
                cin>>reply;
                if(reply == 'Y') wallet += dues;
            }
        }
        else {cout<<"Current pending dues = Rs.0\n";}
        cout<<"Note that these dues are dues for current date. If you have any books borrowed for maore than 15 days, and you have not returned it, your dues will automatically increase by Rs.10 per book per day. However, dues would not increase after you have returned all overdue books.\n";
    }
    void show_history(void){
        if(history.empty()){
            cout<<"No borrowing history!\n";
            return;
        }
        for(auto tr:history){
            if(tr.action=='b') cout<<"Borrowed book ";
            else cout<<"Returned book ";
            cout<<tr.bookid<<" on "<<tr.date<<endl;
        }
    }
};


class faculty : public user {
    private:
    int current_borrowed[5];
    string dates_borrowed[5];
    vector<transaction> history;
    const static int limit=5;
    public:
    faculty(int userid, string uname){
        userID=userid;
        username=uname;
        role='F';
        for(int i=0; i<limit; i++){
            current_borrowed[i]=0;
            dates_borrowed[i]="00-00-0000";
        }
    }
    faculty (string data){
        role=data[0];
        userID = stoi(data.substr(1,u));
        int i=u+2;
        while(i<data.size()&& data[i]!='|'){password.push_back(data[i]); i++;}
        i++;
        while(i<data.size()&& data[i]!='|'){username.push_back(data[i]); i++;}
        i++;
        for(int j=0; j<5; j++){
            current_borrowed[j] = stoi(data.substr(i,b));
            i+=(b+1);
        }
        for(int j=0; j<limit; j++){
            while(i<data.size()&& data[i]!='|'){dates_borrowed[j].push_back(data[i]); i++;}
            i++;
        }
        string tran;
        while(data[i]!='~'){
            if(data[i]=='|') {history.push_back(transaction (tran)); tran.clear();}
            else tran.push_back(data[i]);
            i++;
        }
    }
    string update_database(void){
        string data;
        data.push_back(role);
        data.append(to_string(userID));
        data.push_back('|');
        data.append(password);
        data.push_back('|');
        data.append(username);
        data.push_back('|');
        for(int j=0; j<limit; j++){
            if(current_borrowed[j]==0) data.append("0000");
            else data.append(to_string(current_borrowed[j]));
            data.push_back('|');
        }
        for(int j=0; j<limit; j++){data.append(dates_borrowed[j]); data.push_back('|');}
        for(auto tr:history) {data.append(tr.encode()); data.push_back('|');}
        data.push_back('~');
        data.push_back('\n');
        return data;
    }
    bool borrow(vector<book>& bookdb, string today){
        // check if 5 books issued
        int freeslot=-1;
        for(int i=0; i<limit; i++) {
            if(current_borrowed[i]==0) freeslot=i;
            else if(calculateDaysDifference(dates_borrowed[i],today)>60) {cout<<"Book ID " << current_borrowed[i] <<" borrowed for more than 60 days. Please return this book and try again."; return false;}
        }
        if(freeslot==-1) {cout<<"borrow limit exceeded. Please return a borrowed book and try again."; return false;}
        
        cout<<"Enter Book ID : ";
        int bookid;
        cin>>bookid;
        if(bookid==0) {cout<<"Invalid book ID!\n"; return false;}
        int i=0;
        for(; i<bookdb.size(); i++){
            if((bookdb[i].bookID)==bookid) break;
        }
        if(i==bookdb.size()) {cout<<"Book not found!\n"; return false;}

        if(bookdb[i].borrow(userID)){
            current_borrowed[freeslot]=bookid;
            dates_borrowed[freeslot]=today;
            transaction tr;
            tr.action='b';
            tr.bookid=bookid;
            tr.date=today;
            history.push_back(tr);
            cout<<"Book borrowed successfully.\n";
            return true;
        } else {
            return false;
        }
    }

    bool return_book(vector<book>& bookdb, string today){
        cout<<"Enter Book ID : ";
        int bookid;
        cin>>bookid;
        if(bookid==0) {cout<<"Invalid book ID!\n"; return false;}
        int reqslot=-1;
        for(int i=0; i<limit; i++){
            if(current_borrowed[i]==bookid){reqslot=i; break;}
        }
        if(reqslot==-1) {cout<<"Book not borrowed by user."; return false;}

        int i=0;
        for(; i<bookdb.size(); i++){
            if((bookdb[i].bookID)==bookid) break;
        }
        if(i==bookdb.size()) {cout<<"Book not found!"; return false;}
        bookdb[i].return_book();
        current_borrowed[reqslot]=0;
        int diff = ((calculateDaysDifference(dates_borrowed[reqslot],today)-30));
        if(diff>0) {
            cout<<"Book has been returned after overdue period. Total overdue period (in days) : "<<diff<<endl;
        }
        dates_borrowed[reqslot]=nulldate;
        transaction tr;
        tr.action='r';
        tr.bookid=bookid;
        tr.date=today;
        history.push_back(tr);
        cout<<"Book returned successfully.\n";
        return true;
    }
    void show_history(void){
        if(history.empty()){
            cout<<"No borrowing history!\n";
            return;
        }
        for(auto tr:history){
            if(tr.action=='b') cout<<"borrowed book ";
            else cout<<"Returned book ";
            cout<<tr.bookid<<" on "<<tr.date<<endl;
        }
    }
};
// faculty class ends

class librarian : public user {
    public:
    librarian(int userid, string uname){
        userID=userid;
        username=uname;
        role='L';
    }
    librarian (string data){
        role=data[0];
        userID = stoi(data.substr(1,u));
        int i=u+2;
        while(i<data.size() && data[i]!='|'){password.push_back(data[i]);i++;}
        i++;
        while(i<data.size() && data[i]!='|'){username.push_back(data[i]);i++;}
        i++;
    }
    string update_database(void){
        string data;
        data.push_back(role);
        data.append(to_string(userID));
        data.push_back('|');
        data.append(password);
        data.push_back('|');
        data.append(username);
        data.push_back('|');
        data.push_back('\n');
        return data;
    }
    bool add_book(vector<book>& bookdb){
        book newbook;
        int isused=1;
        cout<<"Enter BookID : ";
        isused=0;
        cin>>newbook.bookID;
        if(newbook.bookID<1000 || newbook.bookID>=10000) {cout<<"Invalid User ID. User ID should be a 4-digit number with a non-zero digit at Thousand's place.\n"; return false;}
        for(auto& b:bookdb){
            if(b.bookID==newbook.bookID) {isused=1; break;}
        }
        if(isused==0) isused=2;
        else if(isused==1) {cout<<"Book ID already in use."; return false;}
        cout<<"Enter book title : ";
        cin>>newbook.title;
        cout<<"Enter book author name : ";
        cin>>newbook.author;
        cout<<"Enter book publisher : ";
        cin>>newbook.publisher;
        cout<<"Enter year of publishing : ";
        cin>>newbook.year_of_publishing;
        cout<<"Enter ISBN code : ";
        cin>>newbook.ISBN;

        bookdb.push_back(newbook);
        cout<<"Book added successfully!\n";
        return true;
    }

    bool remove_book(vector<book>& bookdb){
        int bookid;
        cout<<"Enter BookID : ";
        cin>>bookid;
        for(auto& b:bookdb){
            if(b.bookID==bookid) {b.bookID=0;}
        }
        cout<<"Book removed successfully!\n";
        return true;
    }

    bool update_book(vector<book>& bookdb){
        int i=0;
        int bookid;
        cout<<"Enter BookID : ";
        cin>>bookid;
        if(bookid==0) {cout<<"Invalid Book ID. Enter another number : "; return false;}
        for(i=0; i<bookdb.size(); i++){
            if(bookdb[i].bookID==bookid) break;
        }
        if(i==bookdb.size()) {cout<<"Book not found."; return false;}
        cout<<"Book title : "<<bookdb[i].title<<endl;
        cout<<"Author name : "<<bookdb[i].author<<endl;
        cout<<"Publisher : "<<bookdb[i].publisher<<endl;
        cout<<"Year of publishing : "<<bookdb[i].year_of_publishing<<endl;
        cout<<"Enter ISBN code : "<<bookdb[i].ISBN<<endl;
        
        char reply;
        cout<<"Do you want to change book title? (Y=yes, N=no) : ";
        cin>>reply;
        if(reply=='Y'){cout<<"Enter new book title : "; cin>>bookdb[i].title;}
        
        cout<<"Do you want to change book author? (Y=yes, N=no) : ";
        cin>>reply;
        if(reply=='Y'){cout<<"Enter new author : "; cin>>bookdb[i].author;}
        
        cout<<"Do you want to change book publisher? (Y=yes, N=no) : ";
        cin>>reply;
        if(reply=='Y'){cout<<"Enter new book publisher : "; cin>>bookdb[i].publisher;}
        
        cout<<"Do you want to change year of publishing? (Y=yes, N=no) : ";
        cin>>reply;
        if(reply=='Y'){cout<<"Enter new year of publishing : "; cin>>bookdb[i].year_of_publishing;}
        
        cout<<"Do you want to change ISBN code? (Y=yes, N=no) : ";
        cin>>reply;
        if(reply=='Y'){cout<<"Enter new ISBN code : "; cin>>bookdb[i].ISBN;}
        
        cout<<"Book details updated successfully!\n";
        return true;
    }

    bool add_user(vector<student>& studdb, vector<faculty>& facdb, vector<librarian>& librndb){
        char role;
        cout<<"Select role of new user (S=Student, F=Faculty, L=Librarian) : ";
        cin>>role;
        if(!(role=='S' || role=='F' || role=='L')) {
            cout<<"Invalid Role! Try again later.";
            return false;
        }
        int isused=1;
        if(role=='S'){
            int userid;
            cout<<"Enter User ID : ";
            cin>>userid;
            isused=0;
            if(userid<100 || userid>=1000) {cout<<"Invalid User ID. User ID should be a 3-digit number with a non-zero digit at Hundred's place.\n"; return false;}
            for(auto& b:studdb){
                if(b.userID==userid) {isused=1; break;}
            }
            for(auto& b:facdb){
                if(b.userID==userid) {isused=1; break;}
            }
            for(auto& b:librndb){
                if(b.userID==userid) {isused=1; break;}
            }
            if(isused==1) {cout<<"User ID already in use. Try again.\n"; return false;}
            string username;
            cout<<"Enter username : ";
            cin>>username;
            student newuser(userid, username);
            string pw, cnf;
            bool pwset=false;
            while(!pwset){
                cout<<"Enter password : ";
                cin>>pw;
                cout<<"Confirm Password : ";
                cin>>cnf;
                pwset = newuser.set_password(pw,cnf);
                if(!pwset) cout<<"Password and confirm password not same. Try again.\n";
            }
            studdb.push_back(newuser);
            cout<<"User added successfully!\n";
            return true;
        }

        else if(role=='F'){
            int userid;
            cout<<"Enter User ID : ";
            cin>>userid;
            isused=0;
            if(userid<100 || userid>=1000) {cout<<"Invalid User ID. User ID should be a 3-digit number with a non-zero digit at Hundred's place.\n"; return false;}
            for(auto& b:studdb){
                if(b.userID==userid) {isused=1; break;}
            }
            for(auto& b:facdb){
                if(b.userID==userid) {isused=1; break;}
            }
            for(auto& b:librndb){
                if(b.userID==userid) {isused=1; break;}
            }
            if(isused==1) {cout<<"User ID already in use. Try again.\n"; return false;}
            string username;
            cout<<"Enter username : ";
            cin>>username;
            faculty newuser(userid, username);
            string pw, cnf;
            bool pwset=false;
            while(!pwset){
                cout<<"Enter password : ";
                cin>>pw;
                cout<<"Confirm Password : ";
                cin>>cnf;
                pwset = newuser.set_password(pw,cnf);
                if(!pwset) cout<<"Password and confirm password not same. Try again.\n";
            }
            facdb.push_back(newuser);
            cout<<"User added successfully!\n";
            return true;
        }

        else if(role=='L'){
            int userid;
            cout<<"Enter User ID : ";
            cin>>userid;
            isused=0;
            if(userid<100 || userid>=1000) {cout<<"Invalid User ID. User ID should be a 3-digit number with a non-zero digit at Hundred's place.\n"; return false;}
            if(userid==userID) {cout<<"Librarian cannot remove their own account."; return false;}
            for(auto& b:studdb){
                if(b.userID==userid) {isused=1; break;}
            }
            for(auto& b:facdb){
                if(b.userID==userid) {isused=1; break;}
            }
            for(auto& b:librndb){
                if(b.userID==userid) {isused=1; break;}
            }
            if(isused==1) {cout<<"User ID already in use. Try again.\n"; return false;}
            string username;
            cout<<"Enter username : ";
            cin>>username;
            librarian newuser(userid, username);
            string pw, cnf;
            bool pwset=false;
            while(!pwset){
                cout<<"Enter password : ";
                cin>>pw;
                cout<<"Confirm Password : ";
                cin>>cnf;
                pwset = newuser.set_password(pw,cnf);
                if(!pwset) cout<<"Password and confirm password not same. Try again.\n";
            }
            librndb.push_back(newuser);
            cout<<"User added successfully!\n";
            return true;
        }
        return false;
    }

    bool remove_user(vector<student>& studdb, vector<faculty>& facdb, vector<librarian>& librndb){
        char role;
        cout<<"Select role of new user (S=Student, F=Faculty, L=Librarian) : ";
        cin>>role;
        while(!(role=='S' || role=='F' || role=='L')) {
            cout<<"Invalid Role! Select Role again (S=Student, F=Faculty, L=Librarian) : ";
            cin>>role;
        }
        if(role=='S'){
            int userid;
            cout<<"Enter User ID : ";
            cin>>userid;
            if(userid==0) {cout<<"Invalid User ID."; return false;}
            for(auto& b:studdb){
                if(b.userID==userid) {b.userID=0;}
            }
            cout<<"User removed successfully!\n";
            return true;
        }

        else if(role=='F'){
            int userid;
            cout<<"Enter User ID : ";
            cin>>userid;
            if(userid==0) {cout<<"Invalid User ID."; return false;}
            for(auto& b:facdb){
                if(b.userID==userid) {b.userID=0;}
            }
            cout<<"User removed successfully!\n";
            return true;
        }
        else if(role=='L'){
            int userid;
            cout<<"Enter User ID : ";
            cin>>userid;
            // if(userid==userID) {cout<<"Librarian cannot remove themselves!"; return false;}
            if(userid==0) {cout<<"Invalid User ID."; return false;}
            for(auto& b:librndb){
                if(b.userID==userid) {b.userID=0; break;}
            }
            cout<<"User removed successfully!\n";
            return true;
        }
        return false;
    }
    bool borrow(vector<book>& bookdb, string today){
        cout<<"Librarian is not allowed to borrow books.\n";
        return false;
    }
    bool return_book(vector<book>& bookdb, string today){
        cout<<"Librarian is not allowed to borrow books. Hence return book is also not applicable.\n";
        return false;
    }
    void show_history(void){
        cout<<"Feature not applicable for librarians.\n";
    }
};



class library{
public:
    vector<student> studdb;
    vector<faculty> facdb;
    vector<librarian> librndb;
    vector<book> booksdb;
    library(){
        ifstream user("user.txt");
        if(!user){
            cout<<"User database not found, please try again later.....";
        }
        ifstream books("books.txt");
        if(!books){
            cout<<"Library database not found, please try again later.....";
        }
        string data;
        while(getline(books,data)){
            booksdb.push_back(book (data));
        }
        while(getline(user,data)){
            if(data[0]=='S') studdb.push_back(student (data));
            else if(data[0]=='F') facdb.push_back(faculty (data));
            else if(data[0]=='L') librndb.push_back(librarian (data));
        }
    }
    
    ~library(){
        ofstream user_end("user.txt", std::ofstream::out | std::ofstream::trunc);
        if(!user_end){
            cout<<"User database not found, please try again later.....";
        }
        ofstream books_end("books.txt", std::ofstream::out | std::ofstream::trunc);
        if(!books_end){
            cout<<"Library database not found, please try again later.....";
        }
        for(book b:booksdb) {if(b.bookID!=0) books_end<< b.update_database();}
        for(student b:studdb) {if(b.userID!=0) user_end<< b.update_database();}
        for(faculty b:facdb) {if(b.userID!=0) user_end<< b.update_database();}
        for(librarian b:librndb) {if(b.userID!=0) user_end<< b.update_database();}
        user_end.close();
        books_end.close();
        cout<<"\nData saved successfully\n";
    }
};

int main(){
    std::time_t t = std::time(nullptr);  // Get current time
    std::tm today_tm = *std::localtime(&t);
    string today = dateToString(today_tm);
    
    library mylib;
    int id=0;
    char role='a';
    string password;
    int index=0; // this is index of user in the respective database
    while(role!='q'){
        int authenticate = 0;
        cout<<"\n\nWelcome to Library Management System, made by Pratul Koolwal, Roll no. 230782, as a submission for C++ assignment for course CS253.\nSelect your role\n(S=Student, F=Faculty, L=Librarian)\n[To exit system press 'q'] : ";
        cin>>role;
        while(!(role=='S' || role=='F' || role=='L')) {
            if(role=='q') break;
            cout<<"Invalid Role! Select Role again (S=Student, F=Faculty, L=Librarian)\n[To exit system press 'q'] : ";
            cin>>role;
        }
        if(role=='q') break;
        

        if(role=='S') {
            while(authenticate!=2){
                cout<<"Enter User ID : ";
                cin>>id;
                if(id==0) {cout<<"Invalid user ID.\n"; continue;}
                for(index=0; index<mylib.studdb.size(); index++){
                    if(mylib.studdb[index].userID==id) {authenticate=1; break;}
                }
                if(authenticate==0) {cout<<"UserID does not exist with this role.\n"; continue;}
                cout<<"Enter Password : ";
                cin>>password;
                if(mylib.studdb[index].verify_password(password)) {
                    cout<<"Authentication Successful!\n";
                    authenticate=2;
                    break;
                }
                else cout<<"User authentication failed! Please try again.\n";
            }
            char menu='a';
            while(menu!='q'){
                cout<<"\nSelect the action you want to perform\n(b = borrow book\nr = return a borrowed book\nd = View and Clear Dues\nh = show my borrowing and return history\nq = quit ) : ";
                cin>>menu;
                if(menu=='b') mylib.studdb[index].borrow(mylib.booksdb, today);
                else if(menu=='r') mylib.studdb[index].return_book(mylib.booksdb, today);
                else if(menu=='d') mylib.studdb[index].check_dues_wallet(today);
                else if(menu=='h') mylib.studdb[index].show_history();
                else if(menu=='q') break;
                else cout<<"Invalid command.\n";
            }
        }
        
        
        else if(role=='F'){
            while(authenticate!=2){
                cout<<"Enter User ID : ";
                cin>>id;
                if(id==0) {cout<<"Invalid user ID.\n"; continue;}
                for(index=0; index<mylib.facdb.size(); index++){
                    if(mylib.facdb[index].userID==id) {authenticate=1; break;}
                }
                if(authenticate==0) {cout<<"UserID does not exist with this role.\n"; continue;}
                cout<<"Enter Password : ";
                cin>>password;
                if(mylib.facdb[index].verify_password(password)) {
                    cout<<"Authentication Successful!\n";
                    authenticate=2;
                    break;
                }
                else cout<<"User authentication failed! Please try again.\n";
            }
            char menu='a';
            while(menu!='q'){
                cout<<"\nSelect the action you want to perform\n(b = borrow book\nr = return a borrowed book\nh = show my borrowing and return history\nq = quit ) : ";
                cin>>menu;
                if(menu=='b') mylib.facdb[index].borrow(mylib.booksdb, today);
                else if(menu=='r') mylib.facdb[index].return_book(mylib.booksdb, today);
                else if(menu=='h') mylib.facdb[index].show_history();
                else if(menu=='q') break;
                else cout<<"Invalid command.\n";
            }
        } 
        
        
        else if(role=='L'){
            while(authenticate!=2){
                cout<<"Enter User ID : ";
                cin>>id;
                if(id==0) {cout<<"Invalid user ID.\n"; continue;}
                for(index=0; index<mylib.librndb.size(); index++){
                    if(mylib.librndb[index].userID==id) {authenticate=1; break;}
                }
                if(authenticate==0) {cout<<"UserID does not exist\n"; continue;}
                cout<<"Enter Password : ";
                cin>>password;
                if(mylib.librndb[index].verify_password(password)) {
                    cout<<"Authentication Successful!\n";
                    authenticate=2;
                    break;
                }
                else cout<<"User authentication failed! Please try again.\n";
            }
            string menu="a";
            while(menu!="q"){
                cout<<"\nSelect the action you want to perform\n(ab = add book\nrb = remove book\nub = update book\nau = add user\nru = remove user\nq = quit ) : ";
                cin>>menu;
                if(menu=="ab") mylib.librndb[index].add_book(mylib.booksdb);
                else if(menu=="rb") mylib.librndb[index].remove_book(mylib.booksdb);
                else if(menu=="ub") mylib.librndb[index].update_book(mylib.booksdb);
                else if(menu=="au") mylib.librndb[index].add_user(mylib.studdb,mylib.facdb,mylib.librndb);
                else if(menu=="ru") mylib.librndb[index].remove_user(mylib.studdb,mylib.facdb,mylib.librndb);
                else if(menu=="q") break;
                else cout<<"Invalid command.\n";
            }
        }
    }
    return 0;
}
