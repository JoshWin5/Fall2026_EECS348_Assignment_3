/*
EECS 348 Assignment 3
Description: A program that takes an file and it reads the emails. It than puts the emails in a priority queue.
Inputs: Have the user input files.
Outputs: The emails that are most important and lets you know any unread emails.
Collaborators/Sources: ChatGPT, Gemini, Assignment 2 Instructions
    Gen AI:
        ChatGPT: https://chatgpt.com/c/6ab1d7e4-af7c-83e9-91df-675ac3587653
        Gemni: https://gemini.google.com/app/1e4a1a92268adad3 
    Ai Chats Used:
        ChatGPT: https://chatgpt.com/c/6ab1dff9-3174-83e9-bf6d-2de31d069a54     
Author: Joshua Nguyen
Creation Date: 9/21/2026
Revision Date: 9/29/2026
Revisions: Looking over ChatGPT code and editing it to better fit assignment
*/
//Full Code is from ChatGPT. Used structure and tweaked a few things

#include <iostream>// Uses cout and cin
#include <fstream>// Allowed to read files
#include <sstream>// Allows to separate strings
#include <vector>//Allows the program to use dynamic arrays
#include <string>//Allows to use strings

using namespace std;//Allows for libary 

// --------------------------------------------------
// EMAIL CLASS
// --------------------------------------------------

class Email
{
private:
    string senderCategory;//Stores category
    string subject;//Stores subject
    string date;//Stores date

    int priority;//Stores priority emails

public:

    // Constructor makes sure it initializes email
    Email(string category, string emailSubject, string emailDate)
    {
        senderCategory = category;//Saves sender
        subject = emailSubject;//Saves subject
        date = emailDate;//Saves date

        // Assign priority based on sender category
        if (senderCategory == "Boss")
        {
            priority = 5;//Has highest priority
        }
        else if (senderCategory == "Subordinate")
        {
            priority = 4;//Makes it second highest priority
        }
        else if (senderCategory == "Peer")
        {
            priority = 3;//Makes it third highest priority
        }
        else if (senderCategory == "ImportantPerson")
        {
            priority = 2;//Makes it fourth highest priority
        }
        else
        {
            priority = 1;//Makes it so everyone else is lowest priority
        }
    }

    // Get sender category
    string getSenderCategory()
    {
        return senderCategory;
    }

    // Get subject
    string getSubject()
    {
        return subject;
    }

    // Get date
    string getDate()
    {
        return date;
    }

    // Get priority
    int getPriority()
    {
        return priority;
    }

    // Display email information
    void display()
    {
        cout << "   Sender: " << senderCategory << endl;//Displays sender
        cout << "   Subject: " << subject << endl;//Displays subject
        cout << "   Date: " << date << endl;//Displays date
    }
};


// --------------------------------------------------
// MAXHEAP CLASS
// --------------------------------------------------

class MaxHeap
{
private:

    // List-based implementation of the heap
    vector<Email> heap;


    // Compare two emails
    // Returns true if email1 should come before email2
    bool hasHigherPriority(Email email1, Email email2)
    {
        // Compare sender categories first
        if (email1.getPriority() > email2.getPriority())
        {
            return true;//Makes email1 higher priority
        }

        if (email1.getPriority() < email2.getPriority())
        {
            return false;//Makes email2 higher priority
        }

        // If sender categories are equal,
        // compare the dates.
        // Newest date gets higher priority.

        int year1 = stoi(email1.getDate().substr(6, 4));//Gets year
        int month1 = stoi(email1.getDate().substr(0, 2));//Gets month
        int day1 = stoi(email1.getDate().substr(3, 2));//Gets day

        int year2 = stoi(email2.getDate().substr(6, 4));//Gets year
        int month2 = stoi(email2.getDate().substr(0, 2));//Gets month
        int day2 = stoi(email2.getDate().substr(3, 2));//Gets day

        //Compares years
        if (year1 > year2)
        {
            return true;//Makes email1 have newer year
        }

        if (year1 < year2)
        {
            return false;//Makes emails2 have newer year
        }

        //If years are equal than compare months
        if (month1 > month2)
        {
            return true;//Email1 has newer month
        }

        if (month1 < month2)
        {
            return false;//Email2 has newer month
        }

        if (day1 > day2)
        {
            return true;//Emails has newer day
        }

        return false;//If every component are equal neither of them are higher priority
    }


    // Move an email upward in the heap into the correct position
    void heapifyUp(int index)
    {
        while (index > 0)//Continue while the current email is not the root
        {
            int parent = (index - 1) / 2;//Calculates index of current email's parent

            //Checks whether current email has higher priority than parent
            if (hasHigherPriority(heap[index], heap[parent]))
            {
                Email temporary = heap[index];//Stores current email

                heap[index] = heap[parent];//Moves parent email down into current position

                heap[parent] = temporary;//Moves current email up into the parent's position

                index = parent;//Updates index to parent's position
            }
            else
            {
                break;//Anything else current email is already at correct position stop
            }
        }
    }


    // Move an email downward in the heap until it is in correct position
    void heapifyDown(int index)
    {
        int size = heap.size();//Stores total number of emails in heap

        //While true the emails continues until it reaches correct position
        while (true)
        {
            int leftChild = (2 * index) + 1;//Calculates index of left child
            int rightChild = (2 * index) + 2;//Calculates index of right child

            int largest = index;//Current email has highest priority

            // Check left child
            if (leftChild < size &&
                hasHigherPriority(heap[leftChild], heap[largest]))
            {
                largest = leftChild;//Left child has highest priority so far
            }

            // Check right child
            if (rightChild < size &&
                hasHigherPriority(heap[rightChild], heap[largest]))
            {
                largest = rightChild;//Right child has the highest priorty so far
            }

            // If the current item is already largest, the heap is in the correct order.
            if (largest == index)
            {
                break;//Stops program if neither child has higher priority
            }

            Email temporary = heap[index];//Temporaily store the current email

            heap[index] = heap[largest];//Move the higher priority child into current position

            heap[largest] = temporary;//Moves current email down to the child's position

            index = largest;//Updates index of new position of the email
        }
    }


public:

    // Add an email to the MaxHeap
    void insert(Email email)
    {
        heap.push_back(email);//Add email to the end of the vector

        int lastIndex = heap.size() - 1;//Find index of the newly added email

        heapifyUp(lastIndex);//Moves new email upward if it has higher priority than its parent
    }


    // Check if the heap is empty
    bool isEmpty()
    {
        return heap.empty();//Return true if there are no emails
    }


    // Return the highest-priority email without removing it
    Email peek()
    {
        return heap[0];//Root of a MaxHeap has the highest priority
    }


    // Remove the highest-priority email
    void removeMax()
    {
        if (heap.empty())
        {
            return;//Stop if there are no emails to remove
        }

        // Move the last email to the root
        heap[0] = heap[heap.size() - 1];

        // Remove the last item
        heap.pop_back();

        // Restore the MaxHeap
        if (!heap.empty())
        {
            heapifyDown(0);//Moves root downward if need be
        }
    }


    // Return the number of emails
    int getCount()
    {
        return heap.size();//Returns number of stored emails
    }
};


// --------------------------------------------------
// CEO INBOX CLASS
// --------------------------------------------------

class CEOInbox
{
private:
    MaxHeap emailQueue;//Create a MaxHeap to store and prioritize CEO's emails

    // Process an EMAIL command
    void processEmail(string emailInformation)
    {
        string category;//Stores category
        string subject;//Stores subject
        string date;//Stores data

        stringstream input(emailInformation);

        // Read sender category
        getline(input, category, ',');

        // Read subject
        getline(input, subject, ',');

        // Read date
        getline(input, date);

        // Create an Email object
        Email newEmail(category, subject, date);

        // Insert email into the MaxHeap
        emailQueue.insert(newEmail);
    }

    // Process a NEXT command
    void processNext()
    {
        if (emailQueue.isEmpty())
        {
            cout << "No unread emails." << endl;//Display a message if empty
            return;//Stop processing NEXT command
        }

        cout << "Next email:" << endl;//Display a heading before showing email

        Email nextEmail = emailQueue.peek();//Get highest priority email without removing it
        nextEmail.display();//Display email's information

        cout << endl;//Prints empty line to separate emails in output
    }

    // Process a READ command
    void processRead()
    {
        if (emailQueue.isEmpty())
        {
            cout << "No unread emails to read." << endl;//Displays error message
            return;//Stops processing
        }

        // Remove the highest-priority email
        emailQueue.removeMax();
    }


    // Process a COUNT command
    void processCount()
    {
        cout << "There are "//Prints string
             << emailQueue.getCount()//Gets number of emails
             <<" emails to read."//Prints string
             << endl;//Next line

        cout << endl;//Prints extra line
    }

public:

    // Read commands from a file
    void readFile(string fileName)
    {
        ifstream inputFile(fileName);//Open file for reading

        //Checks whether the file was successful opened
        if (!inputFile.is_open())
        {
            cout << "Error: Could not open file." << endl;//Display error
            return;//Stops if file can't be opened
        }

        string line;//Stores each line read from file

        // Read the file one line at a time
        while (getline(inputFile, line))
        {

            //Removes extra spaces and windows line-ending characters
            while (!line.empty() && (line.back() == '\r' || line.back()==' ' || line.back() == '\t'))//Checks for any spaces and line endings
            {
                line.pop_back();//Remove line
            }

            //Removes spaces and tabs at begining of line
            while(!line.empty() && (line.front() == ' ' || line.front() == '\t'))//Removes space and fron ending characters
            {
                line.erase(0,1); //Removes starting space
            }

            if (line.empty())//Makes sure to skip empty lines
            {
                continue;//Moves to next line
            }

            // Find the command
            if (line.substr(0, 5) == "EMAIL")
            {
                // Remove "EMAIL "
                string emailInformation = line.substr(6);

                processEmail(emailInformation);//Process email information
            }
            else if (line == "NEXT")//Checks whether the line contains NEXT 
            {
                processNext();//Display the highest priority email
            }
            else if (line == "READ")//Checks whether line contains READ
            {
                processRead();//Remove highest priority emails
            }
            else if (line == "COUNT")//Checks the line if contains COUNT
            {
                processCount();//Display number of unread emails
            }
            else
            {
                //Displays message i command is not recognized
                cout << "Unknown command: "
                     << line
                     << endl;
            }
        }

        inputFile.close();//CLose input file after commands 
    }
};

// --------------------------------------------------
// MAIN PROGRAM
// --------------------------------------------------

int main()
{
    CEOInbox inbox;//Creates CEO Inbox to manage emails
    
    string fileName;//Declare string to store filename

    cout << "Enter the email file: ";//Prints to ask user to input file
    cin >> fileName;//Reads filename

    inbox.readFile(fileName);//Puts filename into inbox

    return 0;//Program finished
}