//
//  main.cpp
//  TestCpp
//
//  Created by Hyrum Carlile on 5/9/16.
//  Copyright © 2016 Hyrum Carlile. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <term.h>
#include <stdio.h>
#include <stdlib.h>
#include <termcap.h>

#include "toDoItem.h"

using namespace std;

const string currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%m/%d/%Y at %X", &tstruct);
    
    return buf;
}
/*void clearscreen()
{
    char buf[1024];
    char *str;
    
    tgetent(buf, getenv("TERM"));
    str = tgetstr("cl", NULL);
    fputs(str, stdout);
}*/
void printList(vector<toDoItem> toPrint, bool isArchive)
{
    //this function will take a vector as input and output to the console the list in the readable format.
    //if the list is the archive, it will use the archiveToString function. If the list is the current list,
    //it will use the regular toString() function. The difference between these functions are found in the comments
    //in the functions themselves in the toDoItem.cpp file.
    
    if (isArchive == false)
    {
        for (int i = 0; i < toPrint.size(); i++)
        {
            cout << toPrint[i].toString(i+1);
            //The counter is used to print the item list number. It starts at 1, is passed into the toString() function,
            //and then increments by one after the toString() function is called. In this case, instead of creating a
            //counter variable, we just use i+1 (to adjust for base zero indexing (we want our list to start at 1 instead of 0)).
        }
        cout << "-------------------------------------------------------------------------------------------------------------" << endl;
        //finishes the list with a closing bottom line (a top line is printed for each item in the list.
    }
    else
    {
        for (int i = 0; i < toPrint.size(); i++)
        {
            cout << toPrint[i].archiveToString();
        }
        cout << "-------------------------------------------------------------------------------------------------------------" << endl;
        //finishes the list with a closing bottom line (a top line is printed for each item in the list.
    }
    
}
void readdarchiveitem(vector<toDoItem> archive, vector<toDoItem> &currentList)
{
    //This function prompts the user for an item id and then adds that item back to the current list
    //the archive remains untouched (no duplicate items/ID's)
    int readdItemId;
    cout << "Enter the id of the item to be added to the list: ";
    cin >> readdItemId;
    string m_dateAndTime = archive[readdItemId - 1].getDateAndTime();
    string m_name = archive[readdItemId - 1].getName();
    string m_description = archive[readdItemId - 1].getDescription();
    string m_dueDate = archive[readdItemId - 1].getDueDate();
    string m_priority = archive[readdItemId - 1].getPriority();
    
    currentList.push_back(toDoItem(m_dateAndTime, m_name, m_description, m_dueDate, m_priority, readdItemId));
    cout << "Successfully added \"" << m_name << "\" back to the list.";
}
int main()
{
    bool programRun = true;
    int selection = 0;
    string itemName;
    string itemDescription;
    string dueDate;
    string priority;
    string dateAndTime;
    int itemId = 0;
    int deleteId = 0;
    vector<toDoItem> toDoList;
    vector<toDoItem> toDoDatabase;
    vector<toDoItem> readableArchive;
    const string DATABASE = "database.txt";
    const string CURRENTLIST = "currentlist.txt";
    const string ARCHIVE = "archive.txt";
    ofstream file_out;
    ofstream database;
    ifstream database_in;
    ofstream readable;
    bool firstRun = true;
    int currentId = 0;
    bool deleteIdFound = false;
    int previousId = 0;
    const string VERSION_NUMBER = "1.2";
    const int MIN_MENU_SELECTION = 1;
    const int MAX_MENU_SELECTION = 6;
    bool isArchive = true;
    
    
    database_in.open(DATABASE);
    if (database_in.is_open()){
        while (!database_in.eof()){
            
            database_in >> itemId;
            database_in.clear();
            database_in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(database_in, dateAndTime);
            getline(database_in, itemName);
            getline(database_in, itemDescription);
            getline(database_in, dueDate);
            getline(database_in, priority);
            if (previousId == itemId){
                break;
            }
            else{
                previousId = itemId;
            }
            toDoDatabase.push_back(toDoItem(dateAndTime, itemName, itemDescription, dueDate, priority, itemId));
        }
        database_in.close();
    }
    
    //Import everything from the save file (created when the program exits)
    ifstream file_in;
    file_in.open(CURRENTLIST);
    if (file_in.is_open()){
        while (!file_in.eof()){
            if (firstRun == true){
                file_in >> currentId;
                firstRun = false;
                file_in.clear();
                file_in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            if (file_in.eof() == true){
                break;
            }
            getline(file_in, dateAndTime);
            getline(file_in, itemName);
            getline(file_in, itemDescription);
            getline(file_in, dueDate);
            getline(file_in, priority);
            file_in >> itemId;
            toDoList.push_back(toDoItem(dateAndTime, itemName, itemDescription, dueDate, priority, itemId));
            file_in.clear();
            file_in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        file_in.close();
    }
    
    cout << "Version: " << VERSION_NUMBER << endl;
    
    while (programRun == true){
        //***********************************************************
        //Main Menu
        cout << "\n***MAIN MENU***" << endl
        << "1-Add Item" << endl
        << "2-Remove Item" << endl
        << "3-View List" << endl
        << "4-View Archive" << endl
        << "5-Re-add Archive Item" << endl
        << "6-Save and Exit Program" << endl;
        cin >> selection;
        
        if (selection == 1){
            cin.clear();
            dateAndTime = currentDateTime();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Enter the item title: ";
            getline(cin, itemName);
            cout << "Enter the item description: ";
            getline(cin, itemDescription);
            cout << "Enter the due date: ";
            getline(cin, dueDate);
            cout << "Enter the priority: ";
            getline(cin, priority);
            toDoList.push_back(toDoItem(dateAndTime, itemName, itemDescription, dueDate, priority, ++currentId));
            toDoDatabase.push_back(toDoItem(dateAndTime, itemName, itemDescription, dueDate, priority, currentId));
            database << toDoList[toDoList.size()-1].getName() << endl
            << toDoList[toDoList.size() - 1].getDescription() << endl
            << toDoList[toDoList.size() - 1].getDueDate() << endl
            << toDoList[toDoList.size() - 1].getPriority() << endl
            << toDoList[toDoList.size() - 1].getId() << endl;
        }
        
        if (selection == 2){
            cout << "Enter the id of the item to be deleted: ";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cin >> deleteId;
            
            for (int i = 0; i < toDoList.size(); i++){
                if (toDoList[i].getId() == deleteId){
                    deleteIdFound = true;
                    cout << "\"" << toDoList[i].getName() << "\" was successfully deleted." << endl;
                    for (int j = i; j < toDoList.size() - 1; j++){
                        toDoList[j] = toDoList[j + 1];
                    }
                    toDoList.pop_back();
                }
            }
            if (deleteIdFound == false){
                cout << "Item with ID# \"" << deleteId << "\" was not found.";
            }
        }
        
        if (selection == 3){
            //int counter = 1;
            if (toDoList.size() > 0){
                for (int i = 0; i < toDoList.size(); i++){
                    cout << toDoList[i].toString(i+1);
                    //counter++;
                }
                cout << "-------------------------------------------------------------------------------------------------------------" << endl << endl;
            }
            else {
                cout << "There are no items currently on the list." << endl << endl;
            }
        }
        
        if (selection == 4)
        {
            printList(toDoDatabase, isArchive);
        }
        
        if (selection == 5)
        {
            readdarchiveitem(toDoDatabase, toDoList);
        }
        
        if (selection == 6){
            //Export everything to a file to save it (will get loaded upon program execution)
            remove(CURRENTLIST.c_str());
            file_out.open(CURRENTLIST);
            if (toDoDatabase.size() > 0){
                currentId = toDoDatabase[toDoDatabase.size() - 1].getId();
                file_out << currentId;
            }
            else {
                file_out << 0;
            }
            for (int i = 0; i < toDoList.size(); i++){
                file_out << endl
                << toDoList[i].getDateAndTime() << endl
                << toDoList[i].getName() << endl
                << toDoList[i].getDescription() << endl
                << toDoList[i].getDueDate() << endl
                << toDoList[i].getPriority() << endl
                << toDoList[i].getId();
            }
            remove(DATABASE.c_str());
            database.open(DATABASE);
            for (int i = 0; i < toDoDatabase.size(); i++){
                database << toDoDatabase[i].getId() << endl
                << toDoDatabase[i].getDateAndTime() << endl
                << toDoDatabase[i].getName() << endl
                << toDoDatabase[i].getDescription() << endl
                << toDoDatabase[i].getDueDate() << endl
                << toDoDatabase[i].getPriority() << endl;
            }
            
            remove(ARCHIVE.c_str());
            readable.open(ARCHIVE);
            for (int i = 0; i < toDoDatabase.size(); i++){
                readable << toDoDatabase[i].archiveToString();
            }
            for (int i = 0; i < readableArchive.size(); i++){
                readable << toDoDatabase[i].archiveToString();
            }
            programRun = false;
        }
    }
    
    return 0;
}
