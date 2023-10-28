#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <string>
using std::string;

//hashTable class
class hashTable
{
	private:
		class entry;
		int tableSize;
		entry** hashArray;
		//add the attributes you need for the class---------------------------------------------------------
		int hash(string); //Figures out where a key belongs in the hash table
		entry* getEntry(string); //Retrieves user for use of other function
	public:
		//add all the functions you need--------------------------------------------------------------------
		hashTable(int); //Constructor
		~hashTable(); //Destructor
		string generateSalt();
		string getSalt(string);
		void addEntry(string, string, string);
		bool validateLogin(string, string);
		bool removeUser(string, string);
};

//Entry class
class hashTable::entry
{
	private:
		string uname;
		string salt;
		string passHash;
	public:
		entry* next; //Public variable that points to the next entry in bucket

		//add the constructor and getter functions-----------------------------------------------------------
		entry(string, string, string); //Constructor
		string getUsername();
		string getSalt();
		string getHashedPwd();
};

#endif