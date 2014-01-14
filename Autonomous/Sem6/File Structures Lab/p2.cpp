/******************************************************************************
* File                : p2.cpp
* Description         : Program to demonstrate the use of fixed length buffers
* Author              : Manas Jayanth (prometheansacrifice)
* Compiler            : g++ (gcc compiler 4.6.3, Ubuntu 12.04)
* Date                : 14 December 2014
******************************************************************************/

#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;

/****************************
Forward declaring FixedLengthBuffer since its a member of class Person
******************************/
class FixedLengthBuffer;


/******************************************************************************
* Class               : Person
*
* Members             : usn[] character array, to hold student's usn
*                       name[] character array to hold student's name
*                       address[] character array to hold student's address
*                       branch[] character array to hold student's branch
*                       college[] character array to hold student's college
*
* Methods             : input() to accept user input
*                       output() to display all records
*                       search() to search a record
*                       modify() to modify a given record
******************************************************************************/
class Person {
private:
    char usn[30];
    char name[30];
    char address[30];
    char branch[30];
    char college[30];

public:
    void input();
    void output();
    void search(char *filename);
    void modify(char * filename);
    friend class FixedLengthBuffer;
};


/******************************************************************************
* Class               : FixedLengthBuffer
*
* Members             : buffer[] character array, the buffer
*                       delim character, delimiter character
*
* Methods             : pack() to pack student details into a fixed length buffer
*                       unpack() (vice-versa)
*                       read() to read from the file
*                       write() to write to a file
******************************************************************************/
class FixedLengthBuffer {
private:
    char buffer[160];
    char delim;

public:
    void unpack(Person &p);
    void pack(Person &p);
    void read(fstream &fs);
    void write(char *filename);
    FixedLengthBuffer();
};

/*****************************************************************************
* Takes student details from standard input
*****************************************************************************/
void Person::input()
{
    cout << "Enter USN: "<<endl;
    gets(usn);
    cout << "Enter the Name"<<endl;
    gets(name);
    cout << "Enter the Address"<<endl;
    gets(address);
    cout << "Enter the Branch"<<endl;
    gets(branch);
    cout << "Enter college"<<endl;
    gets(college);
}

/*****************************************************************************
* Displays student details to standard output
*****************************************************************************/
void Person::output()
{
    cout << "USN: ";
    puts(usn);
    cout << "Name: ";
    puts(name);
    cout << "Address: ";
    puts(address);
    cout << "Branch:";
    puts(branch);
    cout << "College: ";
    puts(college);
}

/*****************************************************************************
* Constructor : Initializes the buffer and sets vertical bar as the default
                delimiter
*****************************************************************************/
FixedLengthBuffer::FixedLengthBuffer()
{
    for(int i = 0; i < 160; i++)
    buffer[i] = '\0';
    delim = '|';
}

/*****************************************************************************
* Method : Packs the fields into a fixed length buffer
*****************************************************************************/
void FixedLengthBuffer::pack(Person &p)
{
    strcpy(buffer, p.usn); strcat(buffer,"|");
    strcat(buffer, p.name); strcat(buffer,"|");
    strcat(buffer, p.address); strcat(buffer,"|");
    strcat(buffer, p.branch); strcat(buffer,"|");
    strcat(buffer, p.college); strcat(buffer,"|");
}

/*****************************************************************************
* Method: Unpacks the character data from the fixed length buffer into the student 
*         class
*****************************************************************************/
void FixedLengthBuffer::unpack(Person &p)
{
    char *ptr = buffer;
    while(*ptr) {
        if(*ptr == '|')
            *ptr = '\0';
        ptr++;
    }
    ptr = buffer;
    strcpy(p.usn, ptr);
    ptr = ptr + strlen(ptr)+1;
    strcpy(p.name, ptr);
    ptr = ptr+strlen(ptr) + 1;
    strcpy(p.address, ptr);
    ptr = ptr + strlen(ptr) + 1;
    strcpy(p.branch, ptr);
    ptr = ptr + strlen(ptr) + 1;
    strcpy(p.college, ptr);
}

/*****************************************************************************
* Method : reads a record from the file
*****************************************************************************/
void FixedLengthBuffer::read(fstream& fs)
{
    fs.read(buffer, sizeof(buffer));
}

/*****************************************************************************
* Method : Writes a record into the file
*****************************************************************************/
void FixedLengthBuffer::write(char *filename)
{
    fstream os(filename, ios::out|ios::app);
    os.write(buffer,sizeof(buffer));
    os.close();
}

/*****************************************************************************
* Method : Searches for a record in the file with USN as the key and displays
           displays its contents if found. 
*****************************************************************************/
void Person::search(char *filename)
{
    int found = 0;
    char key[30];
    FixedLengthBuffer b;
    Person p;
    fstream is(filename,ios::in);
    cout << "Enter the usn of the record to be searched:" << endl;
    cin >> key;
    while(is && !found) {
        b.read(is);
        if(is.eof())
        break;
        b.unpack(p);
        if(strcmp(p.usn,key) == 0) {
            cout << "Record found!!!" << endl;
            p.output();
            found = 1;
        }
    }
    if(!found)
    cout << "Record not found!!!" << endl;
    is.close();
}

/*****************************************************************************
* Method : Modifies a file by searching a record, and if found updates it
*****************************************************************************/
void Person::modify(char* filename)
{
    char key[30];
    cout << "Enter the usn of the record to be modified:"<<endl;
    gets(key);
    FixedLengthBuffer b;
    Person p;
    char tempfile[] = "temp.txt";
    int found = 0;
    fstream is(filename, ios::in);
    fstream tfile(tempfile, ios::out|ios::app);
    while(is) {
        b.read(is);
        if(is.eof())
        break;
        b.unpack(p);
        if(strcmp(p.usn,key) == 0) {
            cout<<"\n Enter the new entry:"<<endl;
            p.input();
            b.pack(p);
            b.write(tempfile);
            found = 1;
        }
        else {
            b.pack(p);
            b.write(tempfile);
        }
    }
    if(!found)
    cout << "The record with the given usn does not exist" << endl;
    remove(filename);
    rename(tempfile, filename);
    is.close();
}

/*****************************************************************************
* Function    : main()
* Parameters  : none
*****************************************************************************/
int main()
{
    int choice = 1;
    Person ob;
    FixedLengthBuffer b;
    char filename[] = "data.txt";
    while(choice < 4)
    {
        cout<<"1: Insert a record"<<endl;
        cout<<"2: Search for a record"<<endl;
        cout<<"3: Modify a record"<<endl;
        cout<<"4: Exit"<<endl;
        cout<<"Enter the choice  ";
        cin>>choice;
        cin.get();
        switch(choice)
        {
            case 1: ob.input();
                b.pack(ob);
                b.write(filename);
                break;
            case 2: ob.search(filename);
                break;
            case 3: ob.modify(filename);
                break;
        }
    }
    return 0;
}