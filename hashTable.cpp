/*
	Title:  hashTable.cpp
	Author:  Tyce Webster & Isaac Essex
    Course: CSC 1310-001
	Date:  10/28/2023
    Due: 11/02/2023
	Purpose: Create a hash table for the purpose of hashing passwords and stores users login credentials
*/
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "sha256.h"
#include "hashTable.h"
using std::string;
using std::cout;

hashTable::hashTable(int tableSize) //Constructor
{
	//you need to add everything else that belongs in the constructor--------------------------------------
	hashArray = new entry*[tableSize]; //Dynamically allocates the array of pointers hashArray
	this->tableSize=tableSize;

	for(int i = 0; i < tableSize; i++)
	{
		hashArray[i] = NULL;
	}

	srand (time(0));	//this needs to be in the constructor, otherwise every user gets the same salt
}

hashTable::~hashTable() //Destructor
{
	//Deletes user from the table by looping through every element and deleting it from the bucket
	for(int i = 0; i < tableSize; i++)
	{
		if(hashArray[i] != NULL)
		{
			delete hashArray[i];
		}

		delete [] hashArray;
	}
}

//don't touch this!
int hashTable::hash(string key)
{
	int sum = 0;
	for(int i = 0; i < key.size(); i++)
		sum += key[i];
	
	return sum % tableSize;
}

//don't touch this!
string hashTable::generateSalt()
{
	string s = "";
	
	for(int i = 0; i < tableSize; i++)
		s = s + char('!' + (rand() % 93));

	return s;
}

string hashTable::getSalt(string userN) //Appends the salt to a password before it is hashed
{
	entry* userEntry;

	userEntry = getEntry(userN); //Finds user associated with the key from the parameter

	//If a user if found and the buckett isn't empty, then function returns the salt of userEntry
	if(userEntry != NULL)
	{
		return userEntry->getSalt();
	}
	else
	{
		cout << "User not found on the table\n";
		return " ";
	}
}

//Dynamically allocates the new entry to the list
void hashTable::addEntry(string val1, string val2, string val3)
{
	entry* existing;
	entry* userEntry;
	int index;
	
	existing = getEntry(val1);

	//Checks if username already exists in the table
	if(existing != NULL)
	{
		cout << "\nThis username has already been taken. Input a different one.";
		//Deletes same username and returns nothing so it doesn't add to the table
		delete existing;
		return; 
	}
	
	//Creates new entry with the parameters
	userEntry = new entry(val1, val2, val3);
	index = hash(val1); //Hashes the index value of the username

	//Adds the new user to a specific bucket
	userEntry->next = hashArray[index];
	hashArray[index] = userEntry;
	
}

//Checks the validity of login credentials
bool hashTable::validateLogin(string userNme, string passWrd)
{
	entry* userEntry;

	userEntry = getEntry(userNme);

	//Check if the user exists
	if(userEntry != NULL)
	{
		//hash the given password and compare to the user's password
		passWrd.append(userEntry->getSalt()); //salt it
		passWrd=sha256(passWrd); //hash it
		
		//Compare the encoded passwords
		if(userEntry->getHashedPwd() == passWrd)
		return true;
		else
		return false;
	}
	else
	{
		return false;
	}
}

bool hashTable::removeUser(string userNme, string passWrd) //Removes user from hash table
{
	entry* curr;
	entry* prev;
	int index;

	index = hash(userNme); //hashes index value of userNme
	curr = hashArray[index]; //Sets curr pointer to the element of current hashArray index
	prev = NULL;

	//Finds the user in the bucket
	while(curr != NULL)
	{
		if(curr->getUsername() == userNme) //Verify user's login information
		{
			//hash the given password and compare to the user's password
			passWrd.append(curr->getSalt()); //salt it
			passWrd=sha256(passWrd); //hash it

			if(curr->getHashedPwd() == passWrd) //Correct information, thus removes user from bucket
			{
				if(prev == NULL) 
				{
					hashArray[index] = curr->next; //User is first in the bucket
				}
				else
				{
					prev->next = curr->next; //User's bucket is in the middle of the table
				}

				delete curr; //Removes curr aka the user and returns true that the user is removed
				return true;
			}
			else
			{
				return false; //Incorrect information, user is not removed
			}
		}

		//Changes table to match new changes
		prev = curr;
		curr = curr->next;
	}

	return false; //User is not found in the bucket
}

//Searches for a given username in the database
hashTable::entry* hashTable::getEntry(string key) 
{
	//pointer for iterating
	entry* ptr;
	int index;

	//find our desired bucket based the username
	index = hash(key);
	ptr = hashArray[index]; //point to the bucket

	//search through the bucket until we reach the end of the bucket or
	//find the user in question
	while(ptr != NULL)
	{
		if(ptr->getUsername() == key) //comparing usernames
		{
			return ptr;
		}

		ptr = ptr->next;
	}

	return NULL; //If the key does not correspond to the table
}

//Constructor for entry that assigns the attributes to each parameter and sets next to NULL 
hashTable::entry::entry(string userName, string userHashPwd, string userSalt)
{
	uname = userName;
	salt = userSalt;
	passHash = userHashPwd;
	next = NULL;
}

string hashTable::entry::getUsername() //Returns the username
{
	return uname;
}

string hashTable::entry::getSalt() //Returns the salt for hashing the password
{
	return salt;
}

string hashTable::entry::getHashedPwd() //Returns the hashed password
{
	return passHash;
}