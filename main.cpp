#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include<sstream>



using namespace std;

class Calendar {
private:
	string startVcalendar;
	string calscale;
	string timezone;
	string endVcalendar;

public:
	Calendar() {
		startVcalendar = "BEGIN:VCALENDAR";
		calscale = "CALSCALE:GREGORIAN";
		timezone = "X-WR-TIMEZONE:America/Chicago";
		endVcalendar = "END:VCALENDAR";
	}//this is set by default for my location

	string getStartVCalendar() {
		return startVcalendar;
	}
	string getEndVCalendar() {
		return endVcalendar;
	}//TODO - likely depricated now since i don't want to instantiate a class to create this
	string getCalscale() {
		return calscale;
	}
	string getTimezone() {
		return timezone;
	}

};//not really necessary at the moment but could be used later to adjust calscale and timezones

class Alert {
protected:
	string begin = "BEGIN:VALARM";
	string end = "END:VALARM";
	string action = "ACTION:DISPLAY";
	string trigger = "TRIGGER:-P0DT";

private:
	string timer;
	bool ifTimer = false;;

public:
	Alert() {
	}//why do i need this for the event default constructor?

	Alert(string alertData) {
		setTimer(alertData);
	}

	void setTimer(string data) {
		if (data != "") {
			string upperData = stringToUpper(data); //converts data to all uppercase
			string wholeData;
			for (int i = 0; i < upperData.size(); i++) {
				if (upperData[i] == 'H') {
					wholeData = upperData + "0M0S";
					timer = wholeData;
					ifTimer = true;
				}//check to see if timer based on hours
				else if (upperData[i] == 'M') {
					wholeData = "0H" + upperData + "0S";
					timer = wholeData;
					ifTimer = true;
				}//check to see if timer based on minutes
				else if (upperData[i] == 'D') {
					timer = "0H30M0S";
					ifTimer = true;
				}
			}//TODO - can definitely revise this to be less redundant; not sure how right now
		}
		else {
			ifTimer = false;
		}//basically if innaccesable, have no timer
	}//scans data input for timer to determine when or if one will be created

	string stringToUpper(string lowerString) {
		string rebuildString;
		for (int i = 0; i < lowerString.size(); i++) {
			rebuildString.push_back(toupper(lowerString[i]));
		}//TODO - could do a check what timer setting is by hours, minutes, or default then adjust string then skipping the for loop in setTimer
		return rebuildString;
	}

	string getTimer() {
		return timer;
	}
	bool getTimerExists() {
		return ifTimer;
	}
	string getTimerBegin() {
		return begin;
	}
	string getTimerEnd() {
		return end;
	}
	string getAction() {
		return action;
	}
	string getTrigger() {
		return trigger;
	}
};

class Event :public Calendar {
protected:
	string begin = "BEGIN:VEVENT";
	string end = "END:VEVENT";
	string valueDate = "VALUE=DATE:";
	string summary = "SUMMARY:";
	string dtstart = "DTSTART";
	string dtend = "DTEND";
private:
	string date;
	string startTime;
	string endTime;
	Alert alerts;
	string summaryEvent;
public:
	Event() {
	}

	void setDate(string theDate) {
		date = theDate;
	}
	void setStartTime(string starttime) {
		startTime = starttime;
	}

	void setEndTime(string endtime) {
		endTime = endtime;
	}

	void setAlert(Alert ticker) {
		alerts = ticker;
	}

	void setEventName(string eventname) {
		summaryEvent = eventname;
	}

	string getDate() {
		return date;
	}
	string getStartTime() {
		return startTime;
	}
	string getEndTime() {
		return endTime;
	}
	Alert getAlert() {
		return alerts;
	}
	string getEventName() {
		return summaryEvent;
	}
	string getEventBegin() {
		return begin;
	}
	string getEventEnd() {
		return end;
	}
	string getSummary() {
		return summary;
	}
	string getDtStart() {
		if (getStartTime() != "") {
			return dtstart + ":";
		}
		else {
			return dtstart + ";" + valueDate;
		}
	}
	string getDtEnd() {
		if (getEndTime() != "") {
			return dtstart + ":";
		}
		else {
			return dtstart + ";" + valueDate;
		}
	}
};

int main() {	
	const char* tasksFile = "B:\\Syncthing\\TextFiles\\TaskICSTemplate.txt"; //TODO: add a way to get it so the user can change the directory/file, deterimes if it needs to add a file
	const char* icalFile = "B:\\Syncthing\\TextFiles\\icalTasks.ics"; //TODO: add a way to get it so the user can change the directory/file, deterimes if it needs to add a file
	list<Event> eventList;
	vector<string> icalItems;

	string line;
	ifstream readFromFile(tasksFile);

	readFromFile.ignore(200, '\n');//basically skips the first line after the first 200 characters; arbitrary number

	int i = 0;

	while (getline(readFromFile, line)) {
		icalItems.push_back(line);
		string item = icalItems.at(i);

		stringstream stream(item);

		vector<string> parsedLineItems;

		while (stream.good()) {
			string substr;
			getline(stream, substr, ',');
			parsedLineItems.push_back(substr);
		}//parses the line by each comma and thows them into an array

		Event newEvent;
		newEvent.setDate(parsedLineItems.at(0));
		newEvent.setStartTime(parsedLineItems.at(1));
		newEvent.setEndTime(parsedLineItems.at(2));
		newEvent.setAlert(Alert(parsedLineItems.at(3)));
		newEvent.setEventName(parsedLineItems.at(4));

		eventList.push_back(newEvent);

		i++;
	}//builds the list of events in the text file

	ofstream writeToFile(icalFile);

	Event ical;

	writeToFile << ical.getStartVCalendar() << endl;
	writeToFile << ical.getCalscale() << endl;
	writeToFile << ical.getTimezone() << endl << endl;

	for (Event ev : eventList) {
		writeToFile << ev.getEventBegin() << endl;
		writeToFile << ev.getDtStart() + ev.getDate() + (ev.getStartTime() == "" ? "" : "T" + ev.getStartTime()) << endl;
		writeToFile << ev.getDtEnd() + ev.getDate() + (ev.getEndTime() == "" ? "" : "T" + ev.getEndTime()) << endl;
		writeToFile << ev.getSummary() + ev.getEventName() << endl << endl;

		if (ev.getAlert().getTimerExists()) {
			writeToFile << ev.getAlert().getTimerBegin() << endl;
			writeToFile << ev.getAlert().getAction() << endl;
			writeToFile << ev.getAlert().getTrigger() + ev.getAlert().getTimer() << endl;
			writeToFile << ev.getAlert().getTimerEnd() << endl << endl;
		}
		writeToFile << ev.getEventEnd() << endl << endl;
	}
	writeToFile << ical.getEndVCalendar() << endl;
	readFromFile.close();
	writeToFile.close();

	return 0;
}
