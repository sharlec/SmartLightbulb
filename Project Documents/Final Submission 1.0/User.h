/* 
 *Author: Kirk Vander Ploeg, Jacob Stirling 
 *Description: Header file for a User object. User will have Name, email, 
 *hashed password, registerTime and lastLogin time
 *Date: November 5, 2017
 */
#ifndef USER_H
#define USER_H


#include <string>
#include <ctime>

/* TO IMPLEMENT LATER:make
   #include <Scheduler> */
#include <Bridge.h>

class User {

	public:
		User(); 					
		User(std::string inEmail, std::string password,
			std::string inFirstName, std::string inLastName);
		User(std::string inEmail, std::string password,
			std::string inFirstName, std::string inLastName, int inPin);

		~User();

		std::string getEmail();
		std::string getHashedPassword();
		std::string getFirstName();
		std::string getLastName();
		int getPin();
		std::time_t getRegisterTime();
		std::time_t getLastLogin();

		/* TO IMPLEMNT LATER:
		Scheduler &getScheduler()*/

		void setEmail(std::string inputEmail);
		void setFirstName(std::string inputFirstName);
		void setLastName(std::string inputLastName);
		void setHashedPassword(std::string inputHashedPassword);
		void setPin(int newPin);

		void setRegisterTime(time_t registerTime);
		void setLastLogin(time_t lastLogin);

		bool addToBridges(Bridge bridge);
		size_t getBridgeSize();
		Bridge &getBridge(int id);
		int getBridgeIdByName(std::string name);
		bool removeBridge(Bridge &bridge);
		void clearBridges();

		std::string hashPassword(std::string password);
		bool comparePassword(std::string originalPassword,
			std::string rewrittenPassword);
		int changePassword(std::string originalPassword,
			std::string newPassword, std::string rewrittenPassword);
		bool checkUsernameFormat(std::string username);
		bool checkPasswordFormat(std::string password);
		bool checkNameFormat(std::string name);
		bool empty();

	private:
		std::string email;
		std::string hashedPassword;
		std::string firstName;
		std::string lastName;
		int pin;

		std::time_t registerTime;
		std::time_t lastLogin;

		std::vector<Bridge> bridges;
};

#endif
