#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
using namespace std;

int po[1];

inline bool check (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

string itos(int i) {
    stringstream ss;
    ss << i;
    return ss.str();
}

int *find(string s) {
    int num = 0;
    for (int c = 0; c < s.length(); c++) {
        if (s[c] == ',') {
            po[num] = c;
            num++;
        }
    }
    int *pointer = po;
    
    cout << pointer << " " << po << endl;
    
    return po;
}

int weekdays(int past, int days) {
    int weekdays = (days / 7) * 2;
    if ((days % 7) == 3) {
        weekdays++;
    }
    else if ((days % 7) >= 4) {
        weekdays = weekdays + 2;
    }
    weekdays = days - weekdays;
    
    return weekdays;
}

string exec(const char* cmd) {      //Gives the ability to set a variable as ouput of system command
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

string newfile = "AAPL/AAPLGR100.csv";
string file = "AAPL/AAPL.csv";
int numofdays = 100;


void runShort() {
    stringstream ss;
    string refdate;
    ifstream reffile(newfile.c_str());
    getline(reffile, refdate);
    refdate = refdate.substr(0, 10);
    ss << "date --utc --date " << refdate << " +%s";
    int currsec = atoi(exec(ss.str().c_str()).c_str());
    cout << "Reference: " << refdate << endl << endl;

    ifstream stepper1(file.c_str());
    string currdate;
    getline(stepper1, currdate);
    currdate = currdate.substr(0, 10);

    double greatest = 0.0;
    double lowest = 1000000.0;
    string greatestdate;
    string lowestdate;

    for (int currlinenum = 1; refdate != currdate; currlinenum++) {
        string spacer;
        ifstream stepper2(newfile.c_str());
        for (int count = 0; count < currlinenum; count++) getline(stepper2, spacer);
        for (int count2 = currlinenum + 1; count2 <= (currlinenum + numofdays); count2++) {
            string line;
            getline(stepper2, line);
            //cout << count2 << " : " << line << endl;
            double value = atof(line.substr(11,-1).c_str());
            if (greatest < value) {
                greatest = value;
                greatestdate = line.substr(0,10);
            }
            if (lowest > value) {
                lowest = value;
                lowestdate = line.substr(0,10);
            }
        }

        int diffindays;
        double green;
        double red;

        ss.str("");
        ss << "date --utc --date " << greatestdate << " +%s";
        int greatestsec = atoi(exec(ss.str().c_str()).c_str());
        diffindays = (currsec - greatestsec) / 86400;
        // cout << diffindays << endl;
        green = numofdays - diffindays;
        green = green / <double>(numofdays);
        green = green * 100.0;

        ss.str("");
        ss << "date --utc --date " << lowestdate << " +%s";
        int lowestsec = atoi(exec(ss.str().c_str()).c_str());
        diffindays = (currsec - lowestsec) / 86400;
        // cout << diffindays << endl;
        red = numofdays - diffindays;
        red = red / <double>(numofdays);
        red = red * 100.0;

        cout << currdate << "," << green << "," << red << endl;

        getline(stepper1, currdate);
        currdate = currdate.substr(0, 10);
    }
}


int main() {
    runShort();
}