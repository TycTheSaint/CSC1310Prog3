#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "sha256.h"
#include "hashTable.h"
using std::string;
using std::cout;

hashTable::hashTable(int tableSize)
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

string hashTable::getSalt(string userN) //NEVER CALLED
{
	entry* userEntry;

	userEntry = getEntry(userN);

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
void hashTable::addEntry(string val1, string val2, string val3) //GOOD?
{
	entry* userEntry;
	int index;

	//Allocate new user and find its bucket
	userEntry = new entry(val1, val2, val3);
	index = hash(val1); //find bucket


	//Check if bucket is empty
	if(hashArray[index] == NULL)
	{
		cout<<"\n\nEmpty Bucket\n\n";
		hashArray[index] = userEntry;
	}
	else
	{
		//If backet is not empty, we must ensure there are no duplicate usernames
		entry* curr;
		curr = hashArray[index];

		//Iterating through list
		while(curr->next != NULL)
		{
			cout<<"\n\n"<<curr->getUsername()<<"\n"<<userEntry->getUsername()<<"\n\n";
			//Check for username duplicity
			if(curr->getUsername()==userEntry->getUsername())
			{
				//Deallocate and return early if the name is already taken
				cout<<"\nThis username has already been taken. Input a different one.";
				delete userEntry;
				return;
			}
			else
			{
			curr = curr->next;
			}
		}

		if(curr->getUsername()==userEntry->getUsername()){
			//Deallocate and return early if the name is already taken
			cout<<"\nThis username has already been taken. Input a different one.";
			delete userEntry;
			return;
		}
		
		curr->next = userEntry;
	}
}

//Checks the validity of login credentials
bool hashTable::validateLogin(string userNme, string passWrd) //password is plain text
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

bool hashTable::removeUser(string userNme, string passWrd) //password is plain text
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
			//hash the given password and compare to the user's password
			passWrd.append(curr->getSalt()); //salt it
			passWrd=sha256(passWrd); //hash it

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
	return false;
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

hashTable::entry::entry(string userName, string userHashPwd, string userSalt)
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