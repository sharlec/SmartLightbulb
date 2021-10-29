/* 
 *Author: Kirk Vander Ploeg, Shi Chang, Chris Steward
 *Description: Header file for application. Allows user to interface with webpage in order to alter different functions on a bridge. 
 *Date: November 5, 2017
 */
#ifndef OURAPPLICATION_H
#define OURAPPLICATION_H

#include <Wt/WApplication>
#include <Wt/WEnvironment>

#include "User.h"
#include "FileController.h"
#include "BridgeController.h"

class OurApplication : public Wt::WApplication
{

public:
	OurApplication(const Wt::WEnvironment& env);
	~OurApplication(void);

private:
	std::string title = "Title";

	
	Wt::WContainerWidget *wholePage;			

	Wt::WContainerWidget *header;
	Wt::WContainerWidget *content;
	Wt::WContainerWidget *footer;		
	
	User user;				//used as session to trace current user
	bool loggedIn;

	BridgeController *bridgeClient;

	//user login and user functions
	void addBridgeToUser(std::string response, Bridge &bridge);
    	void login(Wt::WLineEdit* login_email, Wt::WLineEdit* login_password, Wt::WText* msg);
	void signUp(Wt::WLineEdit* first_name,Wt::WLineEdit* last_name,Wt::WLineEdit* email, Wt::WLineEdit* password, Wt::WLineEdit* passcheck,Wt::WText* reg_msg);
	void edit(Wt::WLineEdit* first_name,Wt::WLineEdit* last_name, Wt::WText *msg);
	void logout();
	void delete_user();
	void change_password(Wt::WLineEdit *old_pass, Wt::WLineEdit *new_pass, Wt::WLineEdit *new_check, Wt::WText *msg);

	void turnoff(Bridge b, int lightId);
	void turnon(Bridge b, int lightId);

	
	void addBridge(Wt::WLineEdit *ipAddress, Wt::WLineEdit *portNumber, Wt::WLineEdit *name, Wt::WLineEdit *username);
	void showBridgeInfo(Bridge &bridge);
	void showAllBridges();
	void deleteBridge(Bridge &bridge);
	void showBridgeLights(std::string injson, Bridge b, Wt::WContainerWidget *div);
	void showBridgeGroups(std::string injson, Bridge b, Wt::WContainerWidget *div);

	void changeLightState(Bridge b, int lightId, Wt::WSpinBox *hue, Wt::WSpinBox *saturation, Wt::WSpinBox *brightness);

	void changeGroupState(Bridge b, int groupId, Wt::WSpinBox *hue, Wt::WSpinBox *saturation, Wt::WSpinBox *brightness);


	void addGroupDialog(Bridge &bridge);
	void addNewGroupAction(Bridge &bridge,Wt::WLineEdit *name);


	void doNothing();
	
	//main page elements
	void addHeader();
	void addContent();
	void addFooter();

	//universal pages
	void showLanding();
	void showHome();
	void showAbout();
	
	//logged out pages
	void showRegister();
	void showLogin();

	//logged in pages
	void showLogout();
	void showAccount();
	void showMyBridges();
};

#endif

