#include <iostream>

using namespace std;

class RelayElement
{
	bool _onoff;
	bool _setTrip;
	int _setVolt;
	int _setTime;
	int _setCurrent;


public:
	RelayElement() // Constructors
	{
		_onoff = false;
		_setTrip = false;
		_setVolt = 0;
		_setTime = 0;
		_setCurrent = 0;
	}
	void onRelay(bool in_onoff)
	{
		_onoff = in_onoff;
	}
	void setRelay(bool in_trip,int in_volt, int in_time)
	{
		_setTrip = in_trip;
		_setVolt = in_volt;
		_setTime = in_time;
	}
	bool getTrip()
	{
		return _setTrip;
	}
	bool getOnoff()
	{
		return _onoff;
	}
	int getVolt()
	{
		return _setVolt;
	}
	int getTime()
	{
		return _setTime;
	}
	void printStatus() // Print Relay Status
	{
		cout << "1. Status : " << (_onoff ? "ON" : "OFF") << endl;
		cout << "2. Alarm Type : " << (_setTrip ? "Trip" : "Alarm") << endl;
		cout << "3. Volatge Set Value : " << _setVolt << " V" << endl;
		cout << "4. Time Set Value : " << _setTime << " Sec" << endl;
	}
	void printMenu()
	{
		cout << "-------------------" << endl;
		cout << "-  Relay SETTING  -" << endl;
		cout << "-  1. UVR ( 27 )  -" << endl;
		cout << "-  2. OVR ( 59 )  -" << endl;
		cout << "-  3. Test Relay  -" << endl;
		cout << "-------------------" << endl;
		cout << "Insert Number : ";
	}
	void editSetting()
	{
		cout << "------------------" << endl;
		cout << "Edit Relay setting" << endl;
		cout << "0. No       1. Yes" << endl;
		cout << "Select Number : ";
	}

};
class RelayType
{
public:
	RelayElement uvr;
	RelayElement ovr;
	RelayType(){}
	bool _runOVR = 0;
	bool _runUVR = 0;

	void operateRelay()
	{
		if (_runOVR && ovr.getTrip())
			cout << "[** ALARM **] \n OVR(59) is Operated!!\n[** TRIP **] \n CB is Opened" << endl;
		else if(_runOVR && !ovr.getTrip())
			cout << "[** ALARM **] \n OVR(59) is Operated!!" << endl;
		else if (_runUVR && uvr.getTrip())
			cout << "[** ALARM **] \n UVR(27) is Operated!!\n[** TRIP **] \n CB is Opened" << endl;
		else if (_runUVR && !uvr.getTrip())
			cout << "[** ALARM **] \n UVR(27) is Operated!!" << endl;
		_runOVR = 0;
		_runUVR = 0;
	}

	void compareVR(float test_volt, float test_time)
	{

		if (ovr.getOnoff() && (test_volt >= ovr.getVolt()))
			if (test_time >= ovr.getTime()) {
				_runOVR = 1;
				operateRelay();
			}
		if (uvr.getOnoff() && (uvr.getVolt() >= test_volt))
			if (test_time >= uvr.getTime()) {
				_runUVR = 1;
				operateRelay();
			}
	}
	
};
int main() 
{
	int menu,edit;
	bool in_onoff, in_trip;
	int in_volt, in_time;
	double test_volt, test_time;

	RelayType relay;

	while (1)
	{
		// Relay Select for Setting
		relay.uvr.printMenu();
		cin >> menu;
		cout << endl;

		switch (menu)
		{
		case 1:	// UVR 
			//Show UVR Status
			relay.uvr.printStatus();
			cout << endl;

			relay.uvr.editSetting();
			cin >> edit;
			cout << endl;

			if (edit) // edit UVR value
			{
				//UVR on/off 
				cout << "UVR (27) Relay" << endl;
				cout << "0.off     1.on" << endl;
				cout << "Select Number : ";
				cin >> in_onoff;
				relay.uvr.onRelay(in_onoff);
				cout << endl;

				//Setting UVR Alarm,Volt,Time
				if (in_onoff)
				{
					cout << "UVR(27) Select Alarm Type" << endl;
					cout << "0.Alarm    1.Trip" << endl;
					cout << "Select Number : ";
					cin >> in_trip;
					cout << endl;
					cout << "UVR(27) Setting Voltage(PT Ratio 154000/110)" << endl;
					cout << "Insert Voltage : ";
					cin >> in_volt;
					cout << endl;
					cout << "Insert UVR(27) Relay Setting Time(sec)" << endl;
					cout << "Insert Time : ";
					cin >> in_time;
					cout << endl;
					relay.uvr.setRelay(in_trip, in_volt, in_time);
					cout << "UVR(27) Setting Compelete!" << endl;
					break;
				}
				else
					break;
			}
			else
				break;

		case 2: // OVR case
			relay.ovr.printStatus();
			cout << endl;
			relay.ovr.editSetting();
			cin >> edit;
			cout << endl;

			if (edit)
			{
				cout << "OVR (59) Relay" << endl;
				cout << "0.off     1.on" << endl;
				cout << "Select Number : ";
				cin >> in_onoff;
				relay.ovr.onRelay(in_onoff);
				cout << endl;

				//Setting OVR Alarm,Volt,Time
				if (in_onoff)
				{
					cout << "OVR(59) Select Alarm Type" << endl;
					cout << "0.Alarm    1.Trip" << endl;
					cout << "Select Number : ";
					cin >> in_trip;
					cout << endl;
					cout << "OVR(59) Setting Voltage(PT Ratio 154000/110)" << endl;
					cout << "Insert Voltage : ";
					cin >> in_volt;
					cout << endl;
					cout << "Insert OVR(59) Relay Setting Time(sec)" << endl;
					cout << "Insert Time : ";
					cin >> in_time;
					cout << endl;
					relay.ovr.setRelay(in_trip, in_volt, in_time);
					cout << "OVR(59) Setting Compelete!" << endl;
					break;
				}
				else
					break;
			}
			else
				break;
		case 3: //Test Mode
			//Input Test Volt and Time
			cout << "Test Relay" << endl;
			cout << "Insert Volt : ";
			cin >> test_volt;
			cout << "Insert Time : ";
			cin >> test_time;
			
			// Run Relay
			relay.compareVR(test_volt, test_time);

		}
	}
	return 0;
}