#ifndef BRIDGE_CONTROLLER_H
#define BRIDGE_CONTROLLER_H

#include <string>
#include <vector>

#include <Wt/Json/Object>
#include <Wt/Http/Client>
#include <Wt/WApplication>



#include "Bridge.h"

class BridgeController{
	
	private:
		Wt::Signal<std::string> done1_;
	public:
		BridgeController();
		~BridgeController();
		
		/*int errorCode(std::string jsonString);
		std::string errorDescription(int errorCode);
*/

		void return_json(std::string output);
		Wt::Signal<std::string>& done1();

		void getAllLights(Bridge bridge); //done
		void getLightName(Bridge bridge, int lightID);
		void getLightStates(Bridge bridge, int lightID, int state);

		void createBridgeUser(Bridge &bridge);

		void setLightName(Bridge bridge, int lightID, std::string newName);
		void setLightStateOn(Bridge bridge, int lightID, bool isOn);
		void setLightStateHue(Bridge bridge, int lightID, int newHue);
		void setLightStateSat(Bridge bridge, int lightID, int newSat);
		void setLightStateBright(Bridge bridge, int lightID, int newBright);
		void setLightStateEffect(Bridge bridge, int lightID, int newEffect);
		
		void getAllGroups(Bridge bridge);
		void getGroupName(Bridge bridge, int groupID);
		void getGroupLights(Bridge bridge, int groupID);
		void getGroupAction(Bridge bridge, int groupID, int action);
//kirk u r doing the following
		void setGroupName(Bridge bridge, int groupID, std::string newName);
		void setGroupLights(Bridge bridge, int groupID, std::vector<int> lights);
		void setGroupActionOn(Bridge bridge, int groupID, bool isOn);
		void setGroupActionHue(Bridge bridge, int groupID, int newHue);
		void setGroupActionSat(Bridge bridge, int groupID, int newSat);
		void setGroupActionBright(Bridge bridge, int groupID, int newBright);
		void setGroupActionEffect(Bridge bridge, int groupID, int newEffect);

		void createGroup(Bridge bridge, std::string name);
		void createGroup(Bridge bridge, std::vector<int> lights, std::string name);
		void deleteGroup(Bridge bridge, int groupID);

		void getAllSchedules(Bridge bridge);
		void getScheduleName(Bridge bridge, int schedID);
		void getScheduleTime(Bridge bridge, int schedID);
		void getScheduleCommand(Bridge bridge, int schedID, int command);

		void setSchedule(Bridge bridge, int schedID, std::string name, std::string localTime, int ID, int newHue,
				 int newSat, int newBright, bool isOn, int effect, bool isGroup, bool updateExisting);
		void deleteSchedule(Bridge bridge, int schedID);
};

#endif

