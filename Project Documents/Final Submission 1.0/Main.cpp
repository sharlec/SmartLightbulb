/* Team #: 08
*  members: Charles, Chris, Jacob, Kirk, Pedro
*  description: to create a program that will manage and Control
*  numerous amounts of lightbulbs via central hubs  or bridges
*  purpose of file: the main file, meant to
*  launch the application unde a Wt environment
*  date completed: November 10,2017
*/

// include Wt application file ( to run app in Wt environment)
#include <Wt/WApplication>

// include the OutApplication header file ( to run our website)
#include "OurApplication.h"
// apply use of namespace for Wt
using namespace Wt;

// WApplication, function used to return the OurApplication
// function under the proper envinronment
WApplication *createApplication(const WEnvironment& env) {

	//return the result of the application to the caller
	return new OurApplication(env);
}

// main function: used to create the application and run it under Wt
int main(int argc, char **argv) {
	// return the results of WRun to the caller
	return WRun(argc, argv, &createApplication);
}
