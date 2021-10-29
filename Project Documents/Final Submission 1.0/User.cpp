/* 
 *Author: Kirk Vander Ploeg and Jacob Stirling
 *Description: Implementation of User. Methods to get/set User information,
 *check the format of a password or username and to compare two passwords. 
 *Also includes function to hash a password
 *Date: November 5, 2017
 */

#include <regex>
#include <string>
#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/Auth/HashFunction>
#include "User.h"
 #include <vector>

using namespace Wt;

/*
 *Function Name:User
 *Description:Default Constructor
 *Parameters:N/A
 *return:N/A
 */
User::User(){
pin=0;
}

/*
 *Function Name:User
 *Description:Constructor to create a User object with and email, password
 *First and Last name. Initializes memeber variables
 *Parameters:string inEmail, string password, string inFirstName, string inLastName
 *return:N/A
 */
User::User(std::string inEmail, std::string password, 
			std::string inFirstName, std::string inLastName){
	email = inEmail;
	hashedPassword = this->hashPassword(password);
	firstName = inFirstName;
	lastName = inLastName;
}

/*
 *Function Name:User
 *Description:Constructor to create a User object with and email, password
 *First name, Last name, and pin. Initializes memeber variables
 *Parameters:string inEmail, string password, string inFirstName, string inLastName, int inPin
 *return:N/A
 */
User::User(std::string inEmail, std::string password, 
			std::string inFirstName, std::string inLastName, int inPin = 0){
	email = inEmail;
	hashedPassword = this->hashPassword(password);
	firstName = inFirstName;
	lastName = inLastName;
	pin = inPin;
} 
	
/*
 *Function Name:~User
 *Description:Default destructor
 *Parameters:N/A
 *return:N/A
 */
User::~User(){}


/*
 *Function Name:get<variable>
 *Description:getter methods to return values of member variables
 *Parameters:N/A
 *return:a string representing the member variable requested or time_t in the
 *case of getRegisterTime and getLastLogin
 */
std::string User::getEmail(){return email;}					
std::string User::getHashedPassword(){return hashedPassword;}
std::string User::getFirstName(){return firstName;}
std::string User::getLastName(){return lastName;}
int User::getPin(){return pin;}
std::time_t User::getRegisterTime(){return registerTime;}
std::time_t User::getLastLogin(){return lastLogin;}


/*
 *Function Name:set<variable>
 *Description:setter methods to adjust values of member variables
 *Parameters:String of the changed value for the specified setter. time_t in 
 *case of GetResiterTime and getLastLogin
 *return: void
 */	
void User::setEmail(std::string inputEmail){email = inputEmail;}
void User::setFirstName(std::string inputFirstName){firstName = inputFirstName;}
void User::setLastName(std::string inputLastName){lastName = inputLastName;}
void User::setHashedPassword(std::string inputHashedPassword){hashedPassword = inputHashedPassword;}
void User::setPin(int inPin = 0){pin = inPin;}
void User::setRegisterTime(time_t inputRegisterTime){registerTime = inputRegisterTime;}
void User::setLastLogin(time_t inputLastLogin){lastLogin = inputLastLogin;}

/*
 *Function Name: hashPassword
 *Description:Takes a string and hashes it to an unreadable seemingly 
 *random string
 *Parameters:string password - the password to be hashed
 *return: returns the hashed version of the password
 */	
std::string User::hashPassword(std::string password){
	const std::string& ms = password;
	const std::string salt = "hello world";
	Wt::Auth::MD5HashFunction func;
  	return func.compute(ms, salt);
}


/*
 *Function Name:comparePassword
 *Description:This method will take two passwords and check that they are equal
 *Parameters:two strings to compare against eachother
 *return: true if match false otherwise
 */
bool User::comparePassword(std::string originalPassword, 
	std::string rewrittenPassword){
		if(originalPassword.compare(rewrittenPassword) == 0){
			return true;
		}else return false;
}


/*
 *Function Name:changePassword
 *Description:This function will change the value of the password variable
 *iif the function is provided with the original password and two matching new
 *passwords
 *Parameters:string enteredPassword - the original passowrd to be changed
 *string newPassword, string rewrittenPassword- the new password twice to check 
 *that they are equal. The hashed password variable is updated if okay
 *return: 0 if valid, 2 if new passwords do not match, 1 if original password
 *does not match
 */	
int User::changePassword(std::string enteredPassword,
			std::string newPassword, std::string rewrittenPassword){
	
	std::string enteredHash = hashPassword(enteredPassword);
	
	if(enteredHash.compare(hashedPassword) == 0){
		if(comparePassword(newPassword, rewrittenPassword) == true){
			hashedPassword = hashPassword(newPassword);
			return 0;
		}return 2;
	}else return 1;

}


/*
 *Function Name:checkUsernameFormat
 *Description:Checks a given string for valid username or email format. Valid
 *format is a string that is xy@xz. Where x is a single number/letter, y is a
 *number/letter/./_ and z is a number/letter/.
 *Parameters:A string which is the candidate username
 *return: true if valid false otherwise
 */
bool User::checkUsernameFormat(std::string username){
   	const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	return std::regex_match(username, pattern);
}


/*
 *Function Name:checkPasswordFormat
 *Description:checks that a valid password has been provided. A valid password
 *is made of numbers and letters and is 6-12 characters long
 *Parameters:string for candidate password
 *return: true if valid format, false otherwise
 */
bool User::checkPasswordFormat(std::string password){
	const std::regex pattern("(\\w+)");
	if((password.length() >= 6) && (password.length() <=	12)){
		return std::regex_match(password, pattern);
	}
	return false;
}


/*
 *Function Name:checkNameFormat
 *Description:Checks to make sure a name is of proper format. A name can be 12
 *characters long and must start with a space. Only letters and spaces are valid
 *Parameters:a string for the candidate name
 *return: true if valid, false otherwise
 */	
bool User::checkNameFormat(std::string name){
	const std::regex pattern("[a-zA-Z][a-zA-Z\\s]+");
	if(name.length() <= 12){
		return regex_match(name, pattern);
	}
	else return false;
}

/*
 *Function Name:empty
 *Description: checks if the user object exists
 *Parameters:N/A
 *return: true if empty, false otherwise
 */	
bool User::empty(){
	return (email.size() == 0);
}

/**
 * @brief Adds the bridge to the Bridge vector in the User class
 * @param The Bridge to add to the Bridge vector in the User class
 * @returns Returns whether the function was successful or not
 */
bool User::addToBridges(Bridge bridge)
{
	this->bridges.push_back(bridge);
	return true;
}

/**
 * @brief Gets the Bridge specified by the id out of the Bridge vector from the User Class.
 * @param The id of the Bridge to get.
 * @returns Returns the Bridge from the vector of bridges contained in the User class
 */
Bridge &User::getBridge(int id)
{
	return bridges[id];
}

/**
 * @brief Returns the id of the first bridge with a given name
 * @param Name of the bridge to be found
 * @returns The id of the first bridge with the same name as the parameter. Returns -1 if not found.
 */
int User::getBridgeIdByName(std::string name)
{
	for (int i = 0; i < getBridgeSize(); i++) {
		Bridge currentBridge = getBridge(i);
		
		std::string currentName = currentBridge.getName();
		
		if (currentName.compare(name) == 0) {			
			return i;
		}
	}
	
	return -1;
}

/**
 * @brief Gets the size of the vector containing the bridges in the User Class
 * @param N/A
 * @returns Returns an unsigned int of the size of the vector of bridge
 */
size_t User::getBridgeSize()
{
	return this->bridges.size();
}

/**
 * @brief Remove a Bridge from the vector of bridges contained in the User Class
 * @param the id of the bridge to remove
 * @returns Returns whether the function was successful or not
 */
bool User::removeBridge(Bridge &bridge)
{
	int count = 0;
	int removeIndex;
	bool found = false;
  	for (std::vector<Bridge>::iterator it = bridges.begin(); it != bridges.end(); ++it){
  		count += 1;
  		Bridge temp = *it;
  		if(temp.getName().compare(bridge.getName()) == 0){

  			removeIndex = count;
  			found = true;
  		}
  	}
  	if(found){
  		bridges.erase(bridges.begin() + removeIndex);
  	return true;
  	}
	return false;
}

/**
 * @brief Clear all bridges from bridges vector
 * @param N/A
 * @returns N/A
 */
void User::clearBridges()
{
	bridges.clear();
}
