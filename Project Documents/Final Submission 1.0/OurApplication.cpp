/**
 * @author Chris Steward, Charles Chang
 * @brief Main source file and GUI controller class,
 * does the general controlling of how the program functions and
 * calls functions from other classes as it needs them
 * @date November 5, 2017
 */

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WWidget>
#include <Wt/WCssStyleSheet>
#include <Wt/WPushButton>
#include <Wt/WDialog>
#include <Wt/WLineEdit>
#include <Wt/WBreak>
#include <Wt/WAnchor>
#include <Wt/WLink>
 #include <string>
#include <vector>
#include <set>
#include <iostream>
#include <Wt/Json/Object>
#include <Wt/Json/Array>
#include <Wt/Json/Value>
#include <Wt/Json/Parser>
#include <Wt/WException>
#include <Wt/WString>

#include "BridgeController.h"
#include "BridgeConnector.h"
#include "Bridge.h"

#include <Wt/Json/Object>
#include <Wt/Http/Client>
#include <Wt/WApplication>


#include <Wt/Json/Object>
#include <Wt/Json/Array>
#include <Wt/Json/Value>
#include <Wt/Json/Parser>
#include <Wt/WException>

#include <Wt/WSpinBox>

#include "OurApplication.h"



/**
 * @brief Constructor for the application class, switches on a few basic
 * things and creates the basic look of the website
 * @param env The enviroment variable for starting the server
 * @returns OurApplicaiton object
 */	
OurApplication::OurApplication(const Wt::WEnvironment& env) : WApplication(env)
{
	bridgeClient = new BridgeController();
	
	this->loggedIn = false;
	
	//set style
	this->useStyleSheet("style.css");
		
	wholePage = new Wt::WContainerWidget(root());
	wholePage->setId("whole-page");
	

	//add 3 sections
	addHeader();
	addContent();
	addFooter();

	//set style
	showLanding();
}


/**
 *@Brief Destructor for OurApplication Objects, does some cleanup of pointers
 *@returns N/A
 */	

OurApplication::~OurApplication()
{
        delete header;
	delete content;
	delete footer;

	delete wholePage;
}

/**
 *@Brief Allows user to register for an account. If successful the user will be store in a persistance file
 *@param takes a first name, last name, email, password, repeated password, and message pointer for error display.
 *@returns N/A
 */	
void OurApplication::signUp(Wt::WLineEdit* first_name, Wt::WLineEdit* last_name,Wt::WLineEdit* email, Wt::WLineEdit* password, Wt::WLineEdit* passcheck, Wt::WText* reg_msg){

	User new_user;

	if(first_name->text().empty() || last_name->text().empty() || password->text().empty() || passcheck->text().empty() || email->text().empty()){
		reg_msg->setText("Please fill all information.");
	}
	else if(!new_user.checkUsernameFormat(email->text().narrow())){
		reg_msg->setText("Email format is not valid.");	
	}
	else if(!new_user.checkPasswordFormat( password->text().narrow())){
		reg_msg->setText("Password format is not valid");		
	}
	else if(!new_user.comparePassword( password->text().narrow(), passcheck->text().narrow()) ){
		reg_msg->setText("Two password does not match.");
	}


	else{
		new_user.setEmail(email->text().narrow());
		new_user.setFirstName(first_name->text().narrow());
		new_user.setLastName(last_name->text().narrow());
		new_user.setHashedPassword(new_user.hashPassword(password->text().narrow()));	
		FileController f_control;
		
		f_control.createUser(new_user);
		reg_msg->setText("New user created.");
	}
}

/**
 *@Brief Checks the users information and credentials and allows them to login if correct
 *@param email (as username), password, and message pointer for error display.
 *@returns N/A
 */	
void OurApplication::login(Wt::WLineEdit* login_email, Wt::WLineEdit* login_password, Wt::WText* msg){
		
	if(login_email->text().empty() || login_password->text().empty()){
		msg->setText("Please fill all blanks.");
	}
	else{
		
		FileController f_control;
		
		User &ref = user;
		std::cout << user.hashPassword(login_password->text().narrow()) << std::endl;
		int flag = f_control.getUser(ref,login_email->text().narrow(),user.hashPassword(login_password->text().narrow()));
		
		//flag for 3 cases, 0-valid inputs, 1-account doesn't exist, 2-wrong password
		if(flag == 0){
			this->loggedIn = true;
			delete header;
			delete content;
			delete footer;
			addHeader();
			addContent();
			addFooter();
			showHome();
		}
		else if(flag == 1){
			msg->setText("Account does not exist, sign up now!");
		}
		else{
			msg->setText("Your password is wrong.");
		}
	}
}


/**
 *@Brief Function called when user wants to edit their information
 *@param first name, last name and pointer for error message location. 
 *@returns N/A
 */
void OurApplication::edit(Wt::WLineEdit* first_name,Wt::WLineEdit* last_name, Wt::WText *msg){
	
	//make sure no empty inputs
	if(first_name->text().empty() || last_name->text().empty()){
		msg->setText("Please fill all information.");
	}

	else{
		//update the information in database
		user.setFirstName(first_name->text().narrow());
		user.setLastName(last_name->text().narrow());
		FileController f_control;
		
		f_control.saveUser(user);
		
		content->clear();
		
		content->addWidget(new Wt::WText("Your information has been updated!"));
		content->addWidget(new Wt::WBreak());				
	}
}

/**
 *@Brief Logs the user out and changes the web page back to the landing
 *@param N/A
 *@returns N/A 
 */	
void OurApplication::logout()
{
	this->loggedIn = false;
	this->user = User();
	delete header;
	delete content;
	delete footer;
	addHeader();
	addContent();
	addFooter();
	showLanding();
}

/**
 *@Brief deletes the users account and puts them back to the landing page
 *@param N/A
 *@returns N/A
 */	
void OurApplication::delete_user(){
	FileController f_controller;
	f_controller.deleteUser(user.getEmail());
	WApplication::instance()->redirect("http://localhost:8080");

}

/**
 *@Brief Changes the users password and does some input validaiton
 *@param pointers to old password, new password, repeated new password, and message for error
 *@returns N/A
 */	
void OurApplication::change_password(Wt::WLineEdit *old_pass,Wt::WLineEdit *new_pass,Wt::WLineEdit *new_check, Wt::WText *msg){
	if(!this->loggedIn)
	{
		return;
	}
	
	if(old_pass->text().empty() || new_pass->text().empty() || new_check->text().empty()){
		msg->setText("Please fill all information.");
	}

	else if(!user.comparePassword(new_pass->text().narrow(), new_check->text().narrow())){
		msg->setText("Your new password does not match the repeat one.");
	}

	else{
		FileController f_control;
		User temp;
		User &ref = temp;
		int flag = f_control.getUser(ref,user.getEmail(),user.hashPassword(old_pass->text().narrow()));
		
		//flag for 3 cases, 0-valid inputs, 1-account doesn't exist, 2-wrong password
		if(flag == 0){
			std::string hash_pass = user.hashPassword(new_pass->text().narrow());
			user.setHashedPassword(hash_pass);
			User &ref2 = user;
			f_control.saveUser(ref2);
			
			content->clear();
		
			content->addWidget(new Wt::WText("Your password has been changed!"));
			content->addWidget(new Wt::WBreak());			
		}
		else{
			msg->setText("Your old password is wrong.");
		}
	}	
}

/**
 *@Brief helper function to hook up the GUI to the backend for adding the bridges
 *@param pointers for ip address, port number, name, and username of a bridge
 *@returns N/A
 */
void OurApplication::addBridge(Wt::WLineEdit *ipAddress, Wt::WLineEdit *portNumber, Wt::WLineEdit *name, Wt::WLineEdit *username)
{
	Bridge b = Bridge(name->text().narrow(),
			  ipAddress->text().narrow(),
			  portNumber->text().narrow(),
			  username->text().narrow());
	this->bridgeClient->createBridgeUser(b);
	bridgeClient->done1().connect(boost::bind(&OurApplication::addBridgeToUser, this, _1, b));
}



/**
 *@Brief helper function add bridge to persistance file and update gui
 *@param string response from http request, bridge reference to add to file
 *@returns N/A
 */
void OurApplication::addBridgeToUser(std::string response, Bridge &bridge){
	std::cout << "REACHEDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD!@#!@#!@#!@#!@#!@#!@3" << std::endl;
	
	Wt::Json::Object jObj;
	Wt::Json::parse(response, jObj);

	if(!jObj.contains("success")){
		std::cout << "Bad request!@#!@#!@#!@#!@#!@#!@3" << std::endl;
	} else {
		std::cout << "GOOD request!@#!@#!@#!@#!@#!@#!@3" << std::endl;
		Wt::Json::Object temp = jObj.get("success");
		bridge.setUserName(temp.get("username"));
		std::cout << bridge.getUserName() << std::endl;
		if(this->user.getBridgeIdByName(bridge.getName())==-1){		
		this->user.addToBridges(bridge);
		}
		FileController fcntrl;
		fcntrl.saveUser(this->user);
	}
}


/**
 *@Brief helper function to display information of a single bridge
 *@param reference to bridge to display bridge info
 *@returns N/A
 */
void OurApplication::showBridgeInfo(Bridge &bridge)
{
	this->content->clear();
	Wt::WPushButton *back = new Wt::WPushButton("back to Bridges", content);
	back->clicked().connect(boost::bind(&OurApplication::showMyBridges, this));

	Wt::WContainerWidget *bridgeInfo = new Wt::WContainerWidget(this->content);

	Wt::WText *name = new Wt::WText("Bridge Name: " + bridge.getName());
	Wt::WText *ip = new Wt::WText("IP Address: " + bridge.getIpAddress());
	Wt::WText *port = new Wt::WText("Port: " + bridge.getPort());
	Wt::WText *username = new Wt::WText("Username: " + bridge.getUserName());
	bridgeInfo->addWidget(name);
	bridgeInfo->addWidget(new Wt::WBreak());
	bridgeInfo->addWidget(ip);
	bridgeInfo->addWidget(new Wt::WBreak());
	bridgeInfo->addWidget(port);
	bridgeInfo->addWidget(new Wt::WBreak());
	bridgeInfo->addWidget(username);
	bridgeInfo->addWidget(new Wt::WBreak());
	bridgeInfo->addWidget(new Wt::WBreak());
	bridgeInfo->addWidget(new Wt::WBreak());

	Wt::WContainerWidget *lightInfo = new Wt::WContainerWidget(this->content);
	Wt::WText *lights = new Wt::WText("Lights in " + bridge.getName());
	lightInfo->addWidget(lights);
	BridgeController *getLights = new BridgeController();
	getLights->getAllLights(bridge);
	getLights->done1().connect(boost::bind(&OurApplication::showBridgeLights, this, _1, bridge, lightInfo));


	Wt::WContainerWidget *groupInfo = new Wt::WContainerWidget(this->content);
	Wt::WText *groups = new Wt::WText("Groups in " + bridge.getName());

	BridgeController *bCon = new BridgeController();
	bCon->getAllGroups(bridge);
	bCon->done1().connect(boost::bind(&OurApplication::showBridgeGroups, this, _1, bridge, groupInfo));


	Wt::WPushButton *show = new Wt::WPushButton("show", content);
	show->clicked().connect(boost::bind(&OurApplication::doNothing, this));


	Wt::WPushButton *addDialog = new Wt::WPushButton("Add New Group", content);
	addDialog->clicked().connect(boost::bind(&OurApplication::addGroupDialog, this,bridge));
	
}


/**
 *@Brief displays a pop out window to create a group
 *@param reference to bridge for information in pop out window
 *@returns N/A
 */
void OurApplication::addGroupDialog(Bridge &bridge){
 	

	Wt::WDialog *dialog = new Wt::WDialog("addNewGroup");
	new Wt::WText("Group name: ", dialog->contents());
	
	Wt::WLineEdit *name = new Wt::WLineEdit(dialog->contents());
	
	new Wt::WBreak(dialog->contents());
	Wt::WPushButton *ok = new Wt::WPushButton("submit",dialog->contents());
	
	  
  	ok->clicked().connect(dialog, &Wt::WDialog::accept);

	dialog->finished().connect(boost::bind(&OurApplication::addNewGroupAction,this, bridge,name));
	dialog->show();
}



/**
 *@Brief helper function to add a new action to group
 *@param reference to bridge with actions to be updated, name of the bridge
 *@returns N/A
 */
void OurApplication::addNewGroupAction(Bridge &bridge,Wt::WLineEdit *name){	
	BridgeController *bc = new BridgeController();
	bc->createGroup(bridge, name->text().narrow());
	bc->done1().connect(boost::bind(&OurApplication::doNothing,this));
}

/**
 *@Brief helper function to add bridges to gui
 *@returns N/A
 */
void OurApplication::showAllBridges()
{
	Wt::WContainerWidget *userBridges = new Wt::WContainerWidget(this->content);

	for(size_t i = 0; i < user.getBridgeSize(); i++) {
		Bridge b = user.getBridge(i);


		Wt::WContainerWidget *bridgeDiv = new Wt::WContainerWidget(userBridges);
		Wt::WContainerWidget *infoDiv = new Wt::WContainerWidget(bridgeDiv);

		
		Wt::WPushButton *show = new Wt::WPushButton("Show: " + b.getName(), infoDiv);
		show->clicked().connect(boost::bind(&OurApplication::showBridgeInfo, this, b));

		Wt::WPushButton *del = new Wt::WPushButton("Delete " + b.getName(), infoDiv);
		del->clicked().connect(boost::bind(&OurApplication::deleteBridge, this, b));

	}
}



/**
 *@Brief called when user clicks to delete a bridge. 
 *@param reference to bridge to be deleted
 *@returns N/A
 */
void OurApplication::deleteBridge(Bridge &bridge)
{
	user.removeBridge(bridge);
	showMyBridges();
	FileController fcntrler;
	fcntrler.saveUser(user);
}


/**
 *@Brief helper function that does absolutely nothing except force Wt to rerender when called from a button click
 *@returns N/A
 */
void OurApplication::doNothing(){}

/**
 * @brief sends out request to bridge to change the state of an individual light
 * @params bridge whose lights will be altered, id of light to be changed, value of hue, value of saturation, value of brightness. 
 * @returns N/A
 */
void OurApplication::changeLightState(Bridge b, int lightId, Wt::WSpinBox *hue, Wt::WSpinBox *saturation, Wt::WSpinBox *brightness)
{
	if(hue->value() > 65535){
		hue->setValue(65535);	
	}
	else if (hue->value()<0){
		hue->setValue(0);
	}
	else{}
	

	if(brightness->value() > 254){
		brightness->setValue(254);	
	}
	else if (brightness->value() < 0){
		brightness->setValue(0);
	}
	else{}
	

	if (saturation->value() > 254){
		saturation->setValue(254);	
	}
	else if (saturation->value() < 0){
		saturation->setValue(0);
	}
	else{}
	
	BridgeController *bc = new BridgeController();
	bc->setLightStateHue(b, lightId, hue->value());
	bc->setLightStateSat(b, lightId, saturation->value());
	bc->setLightStateBright(b, lightId, brightness->value());

	std::cout << lightId << " " <<  hue << " " << saturation << " " << brightness << std::endl;
}

/**
 *@Brief helper function to add a bridge's lights to gui
 *@param json reply from bridge, bridge lights are on, div of webpage to update information in. 
 *@returns N/A
 */
void OurApplication::showBridgeLights(std::string injson, Bridge b, Wt::WContainerWidget *div)
{
	std::cout << "**********&&&&&&&&&&&&&&&&&&&&&********" << std::endl;
	Wt::WContainerWidget *lightDiv = new Wt::WContainerWidget(div);
	Wt::Json::Object object;
	Wt::Json::parse(injson, object);
	std::set<std::string> allNames = object.names();

	for(auto it = allNames.begin(); it != allNames.end(); it++){
		Wt::Json::Object lightJson = object[std::string(*it)];
		
		Wt::WText *lightID = new Wt::WText("Light : " + *it + " ");
		lightDiv->addWidget(lightID);
		
		Wt::WText *lightHue = new Wt::WText("Hue: " );
		lightDiv->addWidget(lightHue);
		Wt::WSpinBox *hueInput = new Wt::WSpinBox();
		hueInput->setMinimum(0);				//set the limits
  		hueInput->setMaximum(65535);
		//hueInput->setText();
		lightDiv->addWidget(hueInput);
	
		
		Wt::WText *lightBrightness = new Wt::WText("Brightness: ");
		lightDiv->addWidget(lightBrightness);
		Wt::WSpinBox *brightnessInput = new Wt::WSpinBox();
		brightnessInput->setMinimum(0);				//set the limits
  		brightnessInput->setMaximum(254);  				
		brightnessInput->setText(lightJson["bri"].toString());
		lightDiv->addWidget(brightnessInput);
		

		Wt::WText *lightSaturation = new Wt::WText("Saturation: ");
		lightDiv->addWidget(lightSaturation);
		//saturationInput->setText("0");
		Wt::WSpinBox *saturationInput = new Wt::WSpinBox();
		saturationInput->setMinimum(0);				//set the limits
  		saturationInput->setMaximum(254);  				
		saturationInput->setText(lightJson["sat"].toString());
		lightDiv->addWidget(saturationInput);
	

		Wt::WPushButton *state = new Wt::WPushButton("Update Light-" + lightID->text().narrow(), div);
		state->clicked().connect(boost::bind(&OurApplication::changeLightState, this, b, atoi((*it).c_str()),
						     hueInput, saturationInput, brightnessInput));

		lightDiv->addWidget(new Wt::WBreak());

		Wt::WPushButton *on = new Wt::WPushButton("Light On-" + lightID->text().narrow());
		//on->setId("on");
		Wt::WPushButton *off = new Wt::WPushButton("Light Off-" + lightID->text().narrow(), div);			
		
		on->clicked().connect(boost::bind(&OurApplication::turnon, this, b, atoi((*it).c_str())));
		off->clicked().connect(boost::bind(&OurApplication::turnoff, this, b, atoi((*it).c_str())));
		
		div->insertBefore(off,lightID);
		div->insertBefore(on,off);

		lightDiv->addWidget(new Wt::WBreak());
	}
}


/**
 *@Brief sends information to back end to complete calls to bridge in order to update light on state
 *@param bridge bridge whose lights will be changed, ID number of light to be changed. 
 *@returns N/A
 */
void OurApplication::turnoff(Bridge b, int lightId)
{
	BridgeController *bc = new BridgeController();
	bc->setLightStateHue(b, lightId, 0);
	bc->setLightStateSat(b, lightId, 0);
	bc->setLightStateBright(b, lightId,0);
}


/**
 *@Brief sends information to back end to complete calls to bridge in order to update light off state
 *@param bridge bridge whose lights will be changed, ID number of light to be changed. 
 *@returns N/A
 */
void OurApplication::turnon(Bridge b, int lightId)
{
	
	BridgeController *bc = new BridgeController();
	bc->setLightStateHue(b, lightId, 30000);
	bc->setLightStateSat(b, lightId, 200);
	bc->setLightStateBright(b, lightId,150);
}



/**
 * @brief sends out request to bridge to change the state of a group
 * @params bridge to be changed, id of group to change, updated values for hue, saturation, and brightness
 * @returns N/A
 */
void OurApplication::changeGroupState(Bridge b, int groupId, Wt::WSpinBox *hue, Wt::WSpinBox *saturation, Wt::WSpinBox *brightness)
{

	
	if(hue->value() > 65535){
		hue->setValue(65535);	
	}
	else if (hue->value()<0){
		hue->setValue(0);
	}
	else{}
	

	if(brightness->value() > 254){
		brightness->setValue(254);	
	}
	else if (brightness->value() < 0){
		brightness->setValue(0);
	}
	else{}
	

	if (saturation->value() > 254){
		saturation->setValue(254);	
	}
	else if (saturation->value() < 0){
		saturation->setValue(0);
	}
	else{}
	

	BridgeController *bc = new BridgeController();
	bc->setGroupActionHue(b, groupId, atoi(hue->text().narrow().c_str()));
	bc->setGroupActionSat(b, groupId, atoi(saturation->text().narrow().c_str()));
	bc->setGroupActionBright(b, groupId, atoi(brightness->text().narrow().c_str()));
}

/**
 *@Brief helper function to add bridges's groups to gui
 *@param json reply from bridge request, bridge whose groups are to be displayed, container to display them in
 *@returns N/A
 */
void OurApplication::showBridgeGroups(std::string injson, Bridge b, Wt::WContainerWidget *div)
{


	Wt::WContainerWidget *groupDiv = new Wt::WContainerWidget(div);

	Wt::Json::Object object;
	Wt::Json::parse(injson, object);
	std::set<std::string> allNames = object.names();

	for(auto it = allNames.begin(); it != allNames.end(); it++){

		Wt::Json::Object groupObj = object.get(*it);

		Wt::WText *groupID = new Wt::WText("Group : " + groupObj.get("name").orIfNull("temp") + " ");
		groupDiv->addWidget(groupID);
		
		Wt::WText *lightHue = new Wt::WText("Hue: " );
		groupDiv->addWidget(lightHue);
		Wt::WSpinBox *hueInput = new Wt::WSpinBox();
		hueInput->setMinimum(0);				//set the limits
  		hueInput->setMaximum(65535);  				
		groupDiv->addWidget(hueInput);

		Wt::WText *lightBrightness = new Wt::WText("Brightness: ");
		groupDiv->addWidget(lightBrightness);
		Wt::WSpinBox *brightnessInput = new Wt::WSpinBox();
		brightnessInput->setMinimum(0);				//set the limits
  		brightnessInput->setMaximum(254);  				
		groupDiv->addWidget(brightnessInput);
		

		Wt::WText *lightSaturation = new Wt::WText("Saturation: ");
		groupDiv->addWidget(lightSaturation);
		Wt::WSpinBox *saturationInput = new Wt::WSpinBox();
		saturationInput->setMinimum(0);				//set the limits
  		saturationInput->setMaximum(254);  				
		groupDiv->addWidget(saturationInput);

		
		Wt::WPushButton *state = new Wt::WPushButton("Update Group-" + groupID->text().narrow(), div);
		state->clicked().connect(boost::bind(&OurApplication::changeGroupState, this, b, atoi((*it).c_str()),
						     hueInput, saturationInput, brightnessInput));
		groupDiv->addWidget(new Wt::WBreak());
	}
}

/**
 *@Brief Adds the header to the website including all the buttons and links to functions to change the page
 *@param N/A
 *@returns N/A
 */	
void OurApplication::addHeader()
{
	header = new Wt::WContainerWidget(this->wholePage);
	header->setId("header");

	Wt::WPushButton *homeButton = new Wt::WPushButton("Home", header);
	homeButton->setStyleClass("button");
	homeButton->clicked().connect(this, &OurApplication::showHome);
	
	Wt::WPushButton *aboutButton = new Wt::WPushButton("About Us", header);
	aboutButton->setStyleClass("button");
	aboutButton->clicked().connect(this, &OurApplication::showAbout);

	if(!this->loggedIn) {
		Wt::WPushButton *loginButton = new Wt::WPushButton("Login", header);
		loginButton->setStyleClass("button");
		loginButton->clicked().connect(this, &OurApplication::showLogin);
	
		Wt::WPushButton *registerButton = new Wt::WPushButton("Register", header);
		registerButton->setStyleClass("button");
		registerButton->clicked().connect(this, &OurApplication::showRegister);
	}
	else{
		Wt::WPushButton *accountButton = new Wt::WPushButton("My Account", header);
		accountButton->setStyleClass("button");
		accountButton->clicked().connect(this, &OurApplication::showAccount);

		Wt::WPushButton *myBridgesButton = new Wt::WPushButton("My Bridges", header);
		myBridgesButton->setStyleClass("button");
		myBridgesButton->clicked().connect(this, &OurApplication::showMyBridges);
		
		Wt::WPushButton *logoutButton = new Wt::WPushButton("Logout", header);
		logoutButton->setStyleClass("button");
		logoutButton->clicked().connect(this, &OurApplication::showLogout);

		Wt::WContainerWidget *userDiv = new Wt::WContainerWidget(header);
		userDiv->addWidget(new Wt::WText("Welcome " + user.getFirstName() + " " + user.getLastName()));
		userDiv->setId("whoami");

	}
}



/**
 *@Brief Adds the bare content page to the website
 *@param N/A
 *@returns N/A
 */	
void OurApplication::addContent()
{
	content = new Wt::WContainerWidget(this->wholePage);
	content->setId("content");
	Wt::WText *contentText = new Wt::WText("<p> This is some sweet content</p>");
	content->addWidget(contentText);
}


/**
 *@Brief Adds the footer to the website
 *@param N/A
 *@returns N/A
 */	
void OurApplication::addFooter()
{
	footer = new Wt::WContainerWidget(this->wholePage);
	footer->setId("footer");
	Wt::WText *footerText = new Wt::WText("<p>CS3307 Team 8 - Hue Light Controller Project</p>");
	footer->addWidget(footerText);
}

/**
 *@Brief Shows the landing page with a greeting message
 *@param N/A
 *@returns N/A
 */	
void OurApplication::showLanding()
{
	this->content->clear();
	setTitle("CS3307 Team 8 Home Page");
	content->addWidget(new Wt::WText("<h2>Welcome to CS3307 team08 Phillips Hue Light Control System.</h2>"));  
}

/**
 *@Brief Shows the User's home Page
 *@param N/A
 *@returns N/A
 */	
void OurApplication::showHome()
{
	this->content->clear();

	content->addWidget(new Wt::WText("<h2>Home Sweet Home.</h2>"));  

}

/**
 *@Brief Shows the Login page with input validation 
 *@param N/A
 *@returns N/A
 */	
void OurApplication::showLogin()
{
	this->content->clear();
	
	//adding input box for login
	content->addWidget(new Wt::WText("Email:"));
	Wt::WLineEdit *login_email = new Wt::WLineEdit(this->content); 		             
	login_email->setFocus(); 	

	content->addWidget(new Wt::WText("Password:"));	                
	Wt::WLineEdit *login_password = new Wt::WLineEdit(this->content);	
	login_password->setEchoMode(Wt::WLineEdit::EchoMode(1));                     

	content->addWidget(new Wt::WBreak());

	//adding buttons for login and register
	Wt::WPushButton *login_button = new Wt::WPushButton("login");   
	login_button->setMargin(5, Wt::Left);                            
	content->addWidget(login_button);		

	//adding login msg for invalid input
	Wt::WText *msg = new Wt::WText(content);

	login_button->clicked().connect(boost::bind(&OurApplication::login,this, login_email, login_password,msg));
	login_email->enterPressed().connect(boost::bind(&OurApplication::login,this, login_email, login_password,msg));
	login_password->enterPressed().connect(boost::bind(&OurApplication::login,this, login_email, login_password,msg));	
}

/**
 *@Brief Shows the about page for informaition on it about our team
 *@param N/A
 *@returns N/A
 */	
void OurApplication::showAbout()
{
	this->content->clear();
	
	content->addWidget(new Wt::WText("<h2>About Us</h2>"));  
	content->addWidget(new Wt::WText("<p>We are Team 8 in the CS3307 class and this is our web page for controlling Philips Hue Lights</p>"));
	content->addWidget(new Wt::WText("<h4>Who are We?</h4>"));
	content->addWidget(new Wt::WText("<p>Our team consists of Kirk, Chris, Pedro, Jacob, and Charles. We are all Computer Science Students, Chris and Pedro are in fourth year, and Kirk, Charles, and Jacob are in third year of the Computer Science program.</p>"));
}

/**
 *@Brief Shows the Registration page with all the needed info to make an account
 *@param N/A
 *@returns N/A
 */	
void OurApplication::showRegister()
{
	content->clear();

	setTitle("Register");
	
	//adding input boxes
	content->addWidget(new Wt::WText("First Name:"));
	Wt::WLineEdit *first_name = new Wt::WLineEdit(content);
	first_name->setFocus();
	content->addWidget(new Wt::WBreak());

	content->addWidget(new Wt::WText("Last Name:"));
	Wt::WLineEdit *last_name = new Wt::WLineEdit(content);
	content->addWidget(new Wt::WBreak());

	content->addWidget(new Wt::WText("Email Address:"));
	Wt::WLineEdit *email = new Wt::WLineEdit(content);
	content->addWidget(new Wt::WBreak());

	content->addWidget(new Wt::WText("Password:\t"));
	Wt::WLineEdit *password = new Wt::WLineEdit(content);
	password->setEchoMode(Wt::WLineEdit::EchoMode(1));                     
	content->addWidget(new Wt::WBreak());

	content->addWidget(new Wt::WText("Repeat Password :\t"));
	Wt::WLineEdit *passcheck = new Wt::WLineEdit(content);
	passcheck->setEchoMode(Wt::WLineEdit::EchoMode(1));                     
	content->addWidget(new Wt::WBreak());

	//leave a message to user if invalid input
	Wt::WText *reg_msg = new Wt::WText(content);
	content->addWidget(new Wt::WBreak());
	

	//buttons for signuo and cancel
	Wt::WPushButton *ok = new Wt::WPushButton("Submit", content);
	ok->clicked().connect(boost::bind(&OurApplication::signUp, this, first_name, last_name, email, password, passcheck,reg_msg));	
		
	first_name->enterPressed().connect(boost::bind(&OurApplication::signUp, this, first_name, last_name, email, password, passcheck, reg_msg));
	last_name->enterPressed().connect(boost::bind(&OurApplication::signUp, this, first_name, last_name, email, password, passcheck, reg_msg));
	email->enterPressed().connect(boost::bind(&OurApplication::signUp, this, first_name, last_name, email, password, passcheck, reg_msg));
	password->enterPressed().connect(boost::bind(&OurApplication::signUp, this, first_name, last_name, email, password, passcheck, reg_msg));
	passcheck->enterPressed().connect(boost::bind(&OurApplication::signUp, this, first_name, last_name, email, password, passcheck, reg_msg));
}

/**
 *@Brief Shows the logout page with a single button to logout and end the session
 *@param N/A
 *@returns N/A
 */	
void OurApplication::showLogout()
{
	this->content->clear();

	Wt::WPushButton *logout = new Wt::WPushButton("Logout", content);
	logout->clicked().connect(this, &OurApplication::logout);
}


/**
 *@Brief Shows the account page with the users information and the buttons to change it
 *@param N/A
 *@returns N/A
 */	
void OurApplication::showAccount()
{
	this->content->clear();

	setTitle("Your Information");

	//show account information
	content->addWidget(new Wt::WText("Email: "));
	content->addWidget(new Wt::WText(user.getEmail(),content));
	content->addWidget(new Wt::WBreak());

	//set input boxes
	content->addWidget(new Wt::WText("First Name:"));
	Wt::WLineEdit *first_name = new Wt::WLineEdit(user.getFirstName(),content);
	first_name->setFocus();
	content->addWidget(new Wt::WBreak());


	content->addWidget(new Wt::WText("Last Name:"));
	Wt::WLineEdit *last_name = new Wt::WLineEdit(user.getLastName(),content);
	content->addWidget(new Wt::WBreak());
	
	//set submit and cancel button
	Wt::WText *msg = new Wt::WText(content);
	Wt::WPushButton *ok = new Wt::WPushButton("Submit", content);
	ok->clicked().connect(boost::bind(&OurApplication::edit,this, first_name, last_name,msg));

	first_name->enterPressed().connect(boost::bind(&OurApplication::edit,this, first_name, last_name,msg));
	last_name->enterPressed().connect(boost::bind(&OurApplication::edit,this, first_name, last_name,msg));

	content->addWidget(new Wt::WBreak());
	
	//Change Password stuff
		
	//set input boxes
	content->addWidget(new Wt::WText("Old Password:"));
	Wt::WLineEdit *old_pass = new Wt::WLineEdit(content);
	old_pass->setEchoMode(Wt::WLineEdit::EchoMode(1));   
	old_pass->setFocus();
	content->addWidget(new Wt::WBreak());
	
	content->addWidget(new Wt::WText("New Password:"));
	Wt::WLineEdit *new_pass = new Wt::WLineEdit(content);
	new_pass->setEchoMode(Wt::WLineEdit::EchoMode(1));   
	content->addWidget(new Wt::WBreak());

	content->addWidget(new Wt::WText("Repeat Password:"));
	Wt::WLineEdit *new_check = new Wt::WLineEdit(content);
	new_check->setEchoMode(Wt::WLineEdit::EchoMode(1));   
	content->addWidget(new Wt::WBreak());


	//set submit and cancel button
	msg = new Wt::WText(content);
	Wt::WPushButton *change = new Wt::WPushButton("Submit", content);
	change->clicked().connect(boost::bind(&OurApplication::change_password, this, old_pass, new_pass, new_check,msg));

	content->addWidget(new Wt::WBreak());

	Wt::WPushButton *deleteButton = new Wt::WPushButton("Delete Acount", content);
	deleteButton->clicked().connect(this, &OurApplication::delete_user);
}

/**
 * @brief Shows the My Bridges page and all the content on it
 * @param N/A
 * @return N/A
 */
void OurApplication::showMyBridges()
{
	this->content->clear();
	showAllBridges();
	
	content->addWidget(new Wt::WText("IP Address"));
	Wt::WLineEdit *ipAddress = new Wt::WLineEdit(content);
	content->addWidget(new Wt::WBreak());

	content->addWidget(new Wt::WText("Port Number"));
	Wt::WLineEdit *portNumber = new Wt::WLineEdit(content);
	content->addWidget(new Wt::WBreak());

	content->addWidget(new Wt::WText("Name"));
	Wt::WLineEdit *name = new Wt::WLineEdit(content);
	content->addWidget(new Wt::WBreak());

	content->addWidget(new Wt::WText("Username"));
	Wt::WLineEdit *username = new Wt::WLineEdit(content);
	content->addWidget(new Wt::WBreak());
	
	Wt::WPushButton *addBridge = new Wt::WPushButton("addBridge", content);
	addBridge->clicked().connect(boost::bind(&OurApplication::addBridge, this, ipAddress, portNumber, name, username));


	Wt::WPushButton *show = new Wt::WPushButton("refresh", content);
	show->clicked().connect(boost::bind(&OurApplication::showMyBridges, this));

}

