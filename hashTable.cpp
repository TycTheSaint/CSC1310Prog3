#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "hashTable.h"
using std::string;
using std::cout;

hashTable::hashTable(int size)
{
	//you need to add everything else that belongs in the constructor--------------------------------------
	tableSize = size; //Sets tableSize to value of size
	hashArray = new entry*[tableSize]; //Dynamically allocates the array of pointers hashArray

	for(int i = 0; i < tableSize; i++)
	{
		hashArray[i] = NULL;
	}

	srand (time(0));	//this needs to be in the constructor, otherwise every user gets the same salt
}

hashTable::~hashTable()
{
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

string hashTable::getSalt(string value) fefeofoweofw
{
	entry* userEntry;

	userEntry = getEntry(value);

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

void hashTable::addEntry(string val1, string val2, string val3) foefoesaf
{
	entry* userEntry;
	int index;

	userEntry = new entry(val1, val2, val3);
	index = hash(val1);

	if(hashArray[index] == NULL)
	{
		hashArray[index] = userEntry;
	}
	else
	{
		entry* curr;
		curr = hashArray[index];

		while(curr->next != NULL)
		{
			curr = curr->next;
		}

		curr->next = userEntry;
	}
}

bool hashTable::validateLogin(string userNme, string passWrd) 
{
	entry* userEntry;

	userEntry = getEntry(userNme);

	if(userEntry != NULL && userEntry->getHashedPwd() == passWrd)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool hashTable::removeUser(string userNme, string passWrd) fbebfuieh
{
	entry* curr;
	entry* prev;
	int index;

	index = hash(userNme);
	curr = hashArray[index];
	prev = NULL;

	while(curr != NULL)
	{
		if(curr->getUsername() == userNme)
		{
			if(curr->getHashedPwd() == passWrd)
			{
				if(prev == NULL)
				{
					hashArray[index] = curr->next;
				}
				else
				{
					prev->next = curr->next;
				}

				delete curr;
				return true;
			}
			else
			{
				return false;
			}
		}

		prev = curr;
		curr = curr->next;
	}
}

hashTable::entry* hashTable::getEntry(string key) fhewoufhwou
{
	entry* ptr;
	int index;

	index = hash(key);
	ptr = hashArray[index];

	while(ptr != NULL)
	{
		if(ptr->getUsername() == key)
		{
			return ptr;
		}

		ptr = ptr->next;
	}

	return NULL; //If the key does not correspond to the table
}

hashTable::entry::entry(string userName, string userSalt, string userHashPwd)
{
	uname = userName;
	salt = userSalt;
	passHash = userHashPwd;
	next = NULL;
}

string hashTable::entry::getUsername()
{
	return uname;
}

string hashTable::entry::getSalt()
{
	return salt;
}

string hashTable::entry::getHashedPwd()
{
	return passHash;
}