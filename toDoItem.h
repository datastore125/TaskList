//
//  toDoItem.h
//  TestCpp
//
//  Created by Hyrum Carlile on 5/9/16.
//  Copyright © 2016 Hyrum Carlile. All rights reserved.
//

#pragma once
#include <string>

using namespace std;

class toDoItem
{
public:
    toDoItem(string m_name, string m_description, string m_dueDate, string m_priority);
    toDoItem(string m_name, string m_description, string m_dueDate, string m_priority, int itemId);
    toDoItem(string m_dateAndTime, string m_name, string m_description, string m_dueDate, string m_priority, int itemId);
    ~toDoItem();
    
    string toString(int counter);
    string archiveToString();
    string getDateAndTime();
    string getName();
    string getDescription();
    string getDueDate();
    string getPriority();
    int getId();
    
private:
    string m_dateAndTime;
    string m_name;
    string m_description;
    string m_dueDate;
    string m_priority;
    static int id;
    int itemId;
}; /* toDoItem_h */
