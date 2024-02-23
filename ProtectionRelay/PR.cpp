#include <iostream>
#include <fstream>	// for print file
#include <chrono>	// for time
#include <iomanip>	// for time sort
#include <sstream>	// for time function (nowTime) on File

using namespace std;

class RelayElement
{
	bool _onoff;
	bool _setTrip;
	double _setVolt;
	double _setTime;
	double _setCurrent;


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
	void setVoltRelay(bool in_trip, double in_volt, double in_time)
	{
		_setTrip = in_trip;
		_setVolt = in_volt;
		_setTime = in_time;
	}
	void setCurRelay(bool in_trip, double in_current, double in_time)
	{
		_setTrip = in_trip;
		_setCurrent = in_current;
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
	int getCurrent()
	{
		return _setCurrent;
	}
	int getTime()
	{
		return _setTime;
	}
	void printStatus() // Print Relay Status
	{
		cout << "------------------" << endl;
		cout << "1. Status : " << (_onoff ? "ON" : "OFF") << endl;
		cout << "2. Alarm Type : " << (_setTrip ? "Trip" : "Alarm") << endl;
		cout << "3. Set Value : " << _setVolt << " V\t" <<_setCurrent<< " A" << endl;
		cout << "4. Time Set Value : " << _setTime << " Sec" << endl;
	}
	void printMenu()
	{
		cout << "-------------------" << endl;
		cout << "-  Relay SETTING  -" << endl;
		cout << "-  1. UVR ( 27 )  -" << endl;
		cout << "-  2. OVR ( 59 )  -" << endl;
		cout << "-  3. OCR (50/51) -" << endl;
		cout << "-  4. Test Relay  -" << endl;
		cout << "-  5. Quit        -" << endl;
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
	RelayElement ocr;

	RelayType(){}
	bool _runOVR = 0;
	bool _runUVR = 0;
	bool _runOCR = 0;


	void testRelay()
	{
		cout << "------------------" << endl;
		cout << "    Test Relay    " << endl;
		cout << "0. No       1. Yes" << endl;
		cout << "Select Number : ";
	}
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
		else if (_runOCR && ocr.getTrip())
			cout << "[** ALARM **] \n OCR(50/51) is Operated!!\n[** TRIP **] \n CB is Opened" << endl;
		else if (_runOCR && !ocr.getTrip())
			cout << "[** ALARM **] \n OCR(50/51) is Operated!!" << endl;
		_runOVR = 0;
		_runUVR = 0;
		_runOCR = 0;
	}

	
	string nowTime() // For Write Time on File
	{
		// 현재 시간을 system_clock으로부터 얻기
		auto now = std::chrono::system_clock::now();

		// 시간을 time_t 타입으로 변환
		auto now_c = std::chrono::system_clock::to_time_t(now);

		// tm 구조체로 변환하여 로컬 시간으로 조정
		std::tm now_tm;

		errno_t err = localtime_s(&now_tm, &now_c);
		stringstream time;
		time << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
		return time.str();
	}
	void saveFile(double test_volt, double test_current, double test_time) // Relay Operated Record
	{
		string name;

		if (_runOVR)
			name = "OVR";
		else if (_runUVR)
			name = "UVR";
		else if (_runOCR)
			name = "OCR";

		ofstream file("Relay Operating Record.txt",ios::app);

		if (file.is_open()) {
			if (name == "OCR")
			{
				file << nowTime() << "\t" << name << " Operated\tOperated Value : "
					<< test_current << "A\tOperated Time : "
					<< test_time << "s" << endl;
			}
			else
			{
				file << nowTime() << "\t" << name << " Operated\tOperated Value : "
					<< test_volt << "V\tOperated Time : "
					<< test_time << "s" << endl;
			}
			file.close();
		}
		else {
			cerr << "Cannot Open File" << endl;
		}

	}
	void runVoltRelay(double test_volt,double test_time)
	{

		if (ovr.getOnoff() && (test_volt >= ovr.getVolt()))
			if (test_time >= ovr.getTime()) {
				_runOVR = 1;
				saveFile(test_volt,0, test_time);
				operateRelay();
			}
		if (uvr.getOnoff() && (uvr.getVolt() >= test_volt))
			if (test_time >= uvr.getTime()) {
				_runUVR = 1;
				saveFile(test_volt, 0, test_time);
				operateRelay();
			}

	}
	void runCurRelay(double test_current, double test_time)
	{

		if (ocr.getOnoff() && (test_current >= ocr.getCurrent()))
			if (test_time >= ocr.getTime()) {
				_runOCR = 1;
				saveFile(0,test_current, test_time);
				operateRelay();
			}
	}

};
int main() 
{
	int menu,edit;
	bool in_onoff, in_trip;
	double in_volt, in_time, in_current;
	double test_volt, test_time, test_current;

	RelayType relay;

	while (1)
	{
		// Relay Select for Setting
		relay.uvr.printMenu();
		cin >> menu;
		cout << endl;
		
		if (menu == 5)	//Quit Program
			break;

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
				cout << "0.OFF     1.ON" << endl;
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
					relay.uvr.setVoltRelay(in_trip, in_volt, in_time);
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
				cout << "0.OFF     1.ON" << endl;
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
					relay.ovr.setVoltRelay(in_trip, in_volt, in_time);
					cout << "OVR(59) Setting Compelete!" << endl;
					break;
				}
				else
					break;
			}
			else
				break;
			//OCR
		case 3: 
			relay.ocr.printStatus();
			cout << endl;
			relay.ocr.editSetting();
			cin >> edit;
			cout << endl;

			if (edit)
			{
				cout << "OCR (50/51) Relay" << endl;
				cout << "0.OFF     1.ON" << endl;
				cout << "Select Number : ";
				cin >> in_onoff;
				relay.ocr.onRelay(in_onoff);
				cout << endl;

				//Setting OVR Alarm,Volt,Time
				if (in_onoff)
				{
					cout << "OCR(50/51) Select Alarm Type" << endl;
					cout << "0.Alarm    1.Trip" << endl;
					cout << "Select Number : ";
					cin >> in_trip;
					cout << endl;
					cout << "OCR(50/51) Setting Current(CT Ratio 400/5)" << endl;
					cout << "Insert Current : ";
					cin >> in_current;
					cout << endl;
					cout << "Insert OCR(50/51) Relay Setting Time(sec)" << endl;
					cout << "Insert Time : ";
					cin >> in_time;
					cout << endl;
					relay.ocr.setCurRelay(in_trip, in_current, in_time);
					cout << "OCR(50/51) Setting Compelete!" << endl;
					break;
				}
				else
					break;
			}
			else
				break;
		case 4: //Test Mode
			//Input Test Volt and Time
			relay.testRelay();
			cin >> edit;
			cout << endl;
			if (edit)
			{
				cout << "Test Relay" << endl;
				cout << "Insert Volt : ";
				cin >> test_volt;
				cout << "Insert Current : ";
				cin >> test_current;
				cout << "Insert Time : ";
				cin >> test_time;

				// Run Relay and Write File
				relay.runVoltRelay(test_volt, test_time);
				relay.runCurRelay(test_current, test_time);
			}
		case 5:	//Quit
			break;
		}
		
	}
	return 0;
}