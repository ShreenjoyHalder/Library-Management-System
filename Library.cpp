#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Library {
private:
    unordered_map<string, pair<vector<string>, int>> record;
    unordered_map<int, vector<string>> user;
    unordered_set<string> authors;

    string show(const vector<string>& l) {
        stringstream ss;
        for(size_t i=0;i<l.size();++i) {
            ss<<l[i];
            if(i!=l.size()-1)
                ss<<", ";
        }
        return ss.str();
    }

public:
    Library(unordered_map<string, pair<vector<string>, int>> d) {
        for(auto& it : d) {
            string key=it.first;
            auto value=it.second;
            record[key]=value;
            for(auto& author : value.first)
                authors.insert(author);
        }
    }

    void display() {
        cout<<"The name of books and their counts are given below\n";
        int c=0;
        for(auto& it : record) {
            string key=it.first;
            auto value=it.second;
            cout<<c++<<". "<<key<<" -- "<<value.second<<"\n";
        }
    }

    void display_with_authors() {
        cout<<"The name of books, authors and their counts are given below\n";
        int c=1;
        for(auto& it : record) {
            string key=it.first;
            auto value=it.second;
            cout<<c++<<". "<<key<<" -- "<<show(value.first)<<" -- "<<value.second<<"\n";
        }
    }

    void add_book(string title, vector<string> authorsList, int count) {
        if(record.find(title)!=record.end()) {
            record[title].second += count;
        } else {
            record[title]={authorsList, count};
            for(auto& author : authorsList)
                authors.insert(author);
        }
        cout<<count<<(count>1 ? " copies" : " copy")<<" of the book "<<title<<" was added to the inventory\n";
    }

    void borrow_book(string title, int id) {
        if(record.find(title)!=record.end()) {
            if(record[title].second>0) {
                if(user.find(id)!=user.end()) {
                    if(user[id].size()<3) {
                        record[title].second--;
                        user[id].push_back(title);
                        cout<<"A copy of the book '"<<title<<"' was issued to user with id "<<id<<"\n";
                    } else {
                        cout<<"Any user can borrow at most 3 books\n";
                    }
                } else {
                    cout<<"There is no user with id "<<id<<"\n";
                }
            } else {
                cout<<"No copy of the book '"<<title<<"' is currently in inventory. All are borrowed\n";
            }
        } else {
            cout<<"The Book '"<<title<<"' is not available\n";
        }
    }

    void return_book(string title, int id) {
        if(record.find(title)!=record.end()) {
            if(user.find(id)!=user.end()) {
                auto& borrowed=user[id];
                auto it=find(borrowed.begin(), borrowed.end(), title);
                if(it!=borrowed.end()) {
                    borrowed.erase(it);
                    record[title].second++;
                    cout<<"A copy of the book '"<<title<<"' was returned by user with id "<<id<<"\n";
                } else {
                    cout<<"User with id "<<id<<" did not borrow the book '"<<title<<"'\n";
                }
            } else {
                cout<<"There is no user with id "<<id<<"\n";
            }
        } else {
            cout<<"There is no such book in our inventory\n";
        }
    }

    void books_of_author(string author) {
        cout<<"The books of "<<author<<" which are available are given below\n";
        int c=1;
        for(auto& it : record) {
            string key=it.first;
            auto value=it.second;
            if(find(value.first.begin(), value.first.end(), author)!=value.first.end()) {
                cout<<c++<<". "<<key<<", available count in the inventory -- "<<value.second<<"\n";
            }
        }
    }

    void all_authors() {
        int c=1;
        for(auto& author : authors) {
            cout<<c++<<". "<<author<<"\n";
        }
    }

    void user_info(int id) {
        if(user.find(id)==user.end()) {
            cout<<"User with id "<<id<<" does not exist\n";
            return;
        }
        cout<<"Books taken by user with id "<<id<<" are given below\n";
        int c=1;
        for(auto& book : user[id]) {
            cout<<c++<<". "<<book<<"\n";
        }
    }

    void add_user(int id) {
        if(user.find(id)!=user.end()) {
            cout<<"id "<<id<<" is already taken\n";
        } else {
            user[id]={};
            cout<<"New user with id "<<id<<" was added\n";
        }
    }

    void search_book(string title) {
        for(auto& it : record) {
            string key=it.first;
            auto value=it.second.second;
            if(key==title) {
                if(value!=0) {
                    cout<<"Currently there are "<<value<<" copies of the book '"<<key<<"'\n";
                }
                else {
                    cout<<"No copy of the book '"<<key<<"' is currently in inventory. All are borrowed\n";
                }
                return;
            }
        }
        cout<<"There is no such book in our inventory\n";
    }
};

int main() {
    unordered_map<string, pair<vector<string>, int>> d={
        {"Pather Panchali", {{"Bibhutibhushan Bandyopadhyay"}, 2}},
        {"Chokher Bali", {{"Rabindranath Tagore"}, 1}},
        {"Devdas", {{"Sarat Chandra Chattopadhyay"}, 2}},
        {"Feluda Series", {{"Satyajit Ray"}, 1}},
        {"Byomkesh Bakshi", {{"Sharadindu Bandyopadhyay"}, 1}},
        {"Shesher Kobita", {{"Rabindranath Tagore"}, 1}},
        {"Kalki Kahini", {{"Satyajit Ray", "Adrish Bardhan"}, 2}},
        {"Gitanjali", {{"Rabindranath Tagore"}, 3}},
        {"Aranyak", {{"Bibhutibhushan Bandyopadhyay"}, 2}},
        {"Parineeta", {{"Sarat Chandra Chattopadhyay"}, 1}},
        {"Hajar Churashir Maa", {{"Mahasweta Devi"}, 2}},
        {"Leela Majumdar Rachana Sangraha", {{"Leela Majumdar"}, 1}},
        {"Ek Dozon Uponyas", {{"Samaresh Majumdar", "Sunil Gangopadhyay", "Shirshendu Mukhopadhyay"}, 2}},
        {"Char Adhyay", {{"Rabindranath Tagore"}, 1}},
        {"Datta", {{"Sarat Chandra Chattopadhyay"}, 2}},
        {"Chander Pahar", {{"Bibhutibhushan Bandyopadhyay"}, 3}},
        {"Kakababu Series", {{"Sunil Gangopadhyay"}, 2}},
        {"Manojder Adbhut Bari", {{"Shirshendu Mukhopadhyay"}, 1}},
        {"Tenida Samagra", {{"Narayan Gangopadhyay"}, 2}},
        {"Bengali Science Fiction Omnibus", {{"Adrish Bardhan", "Premendra Mitra", "Satyajit Ray"}, 2}},
    };

    Library library(d);
    int n=-1;
    cout<<"--- Welcome to our Library Management System ---\n";
    while(n!=0) {
        cout<<"\n1. to display the books and their counts\n"
            <<"2. to display the books and their counts along with authors\n"
            <<"3. to add book\n"
            <<"4. to borrow book\n"
            <<"5. to return book\n"
            <<"6. to get books of an author\n"
            <<"7. to get all the authors\n"
            <<"8. to get information of an user\n"
            <<"9. to add an user\n"
            <<"10. to search book\n"
            <<"0. to exit\n\n";

        cout<<"Please enter your choice : ";
        cin>>n;
        cout<<"\n";
        cin.ignore();

        if(n==1) library.display();
        else if(n==2) library.display_with_authors();
        else if(n==3) {
            string title, authorLine;
            int count;
            cout<<"Title of the book to be added : ";
            getline(cin, title);
            cout<<"Authors of the book to be added(comma separated) : ";
            getline(cin, authorLine);
            cout<<"Count of the books to be added : ";
            cin>>count;
            cin.ignore();
            vector<string> authorsList;
            stringstream ss(authorLine);
            string author;
            while(getline(ss, author, ',')) {
                author.erase(0, author.find_first_not_of(" \t"));// trim
                authorsList.push_back(author);
            }
            library.add_book(title, authorsList, count);
        }
        else if(n==4) {
            string title;
            int id;
            cout<<"Title of the book to be borrowed : ";
            getline(cin, title);
            cout<<"User id : ";
            cin>>id;
            cin.ignore();
            library.borrow_book(title, id);
        }
        else if(n==5) {
            string title;
            int id;
            cout<<"Title of the book to be returned : ";
            getline(cin, title);
            cout<<"User id : ";
            cin>>id;
            cin.ignore();
            library.return_book(title, id);
        }
        else if(n==6) {
            string author;
            cout<<"Name of the author : ";
            getline(cin, author);
            library.books_of_author(author);
        }
        else if(n==7) {
            library.all_authors();
        }
        else if(n==8) {
            int id;
            cout<<"User id : ";
            cin>>id;
            cin.ignore();
            library.user_info(id);
        }
        else if(n==9) {
            int id;
            cout<<"User id : ";
            cin>>id;
            cin.ignore();
            library.add_user(id);
        }
        else if(n==10) {
            string title;
            cout<<"Title of the book to be searched : ";
            getline(cin, title);
            library.search_book(title);
        }
        else if(n==0) {
            cout<<"We appreciate you for using our services\n";
        }
        else {
            cout<<"Please enter a valid response\n";
        }
        cout<<"\n----------------------------------------------------------------------------------\n";
    }

    return 0;
}