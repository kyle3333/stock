#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
using namespace std;
/////////////////////////Initializes variables so they are public//////////////////////////////
string ticker;      //
string file;        //
string newfile;     // 
int linetotal;      //          
int numofdays;      //
int linedivided;    //
                    //
int pos;            //
///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////All of the secondary functions/////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////Checks if file or folder exists/////////////////////////////////
inline bool check (const string& name) {       
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}
//////////////////////////////////Converts int to string///////////////////////////////////////
string itos(int i) {
    stringstream ss;    // Creates stringstream
    ss << i;            // Puts int in stringstream 
    return ss.str();    // Converts stringstream to string and returns
}
////////////////////////////Find where the two commas are located//////////////////////////////
/*int *find(string s) {
    int num = 0;
    for (int c = 0; c < s.length(); c++) {
        if (s[c] == ',') {
            pos[num] = c;
            num++;
        }
    }
    int *pointer = pos;
    
    return pos;         //int *point = find(a);
}*/
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////The full program (does all calcs)///////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void runFull() {
    for (int currlinenum = 1; currlinenum <= (linetotal - numofdays); currlinenum++) {      //Loop through lines
        string line;
        stringstream ss;
        ifstream infile(file.c_str());
        
        string currentdate;
        for (int count2 = 1; count2 < currlinenum; count2++) getline(infile, line);
        getline(infile, currentdate);
        currentdate = currentdate.substr(0,10);
        ss.str("");
        ss << "date --utc --date " << currentdate << " +%s";
        int currentsec = atoi(exec(ss.str().c_str()).c_str());
        
        double greatest = 0.0;
        double lowest = 1000000.0;
        string greatestdate;
        string lowestdate;
        
        for (int count2 = currlinenum + 1; count2 <= (currlinenum + numofdays); count2++) {
            getline(infile, line);
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
        //cout << greatestdate << " " << lowestdate << endl;
        //cout.precision(9);
        //cout << greatest << " " << lowest << endl;
        //string b;
        //getline(cin, b);
        
        int diffindays;
        double green;
        double red;
        
        ss.str("");
        ss << "date --utc --date " << greatestdate << " +%s";
        int greatestsec = atoi(exec(ss.str().c_str()).c_str());
        diffindays = (currentsec - greatestsec) / 86400;
        //cout << diffindays << endl;
        green = numofdays - diffindays;
        green = green / double(numofdays);
        green = green * 100.0;
        
        ss.str("");
        ss << "date --utc --date " << lowestdate << " +%s";
        int lowestsec = atoi(exec(ss.str().c_str()).c_str());
        diffindays = (currentsec - lowestsec) / 86400;
        //cout << diffindays << endl;
        red = numofdays - diffindays;
        red = red / double(numofdays);
        red = red * 100.0;
        
        //cout << greatestsec << " " << lowestsec << endl;
        
        stringstream out;
        out << "echo " << currentdate << "," << green << "," << red << " >> " << newfile;
        system(out.str().c_str());
        
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Only calculates the new data/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void runShort() {           // Logic to only add new lines of data 
    string line;
    ifstream infile(newfile.c_str());       //Green Red File
    getline(infile, line);
    string currentfiledate = line.substr(0,10);
    
    stringstream out;
    ifstream infile(file.c_str());          //Blue File
    getline(infile, line);
    for (int currlinenum = 1; line.substr(0,10) != currentfiledate; currlinenum++) {
        ifstream infile2(file.c_str());
        string line2;
        double greatest = 0.0;
        double lowest = 1000000.0;
        string greatestdate;
        string lowestdate;
        
        string spacer
        string currentdate;
        for (int count2 = 1; count2 < currlinenum; count2++) getline(infile, spacer);
        getline(infile, currentdate);
        currentdate = currentdate.substr(0,10);
        ss.str("");
        ss << "date --utc --date " << currentdate << " +%s";
        int currentsec = atoi(exec(ss.str().c_str()).c_str());
        
        for (int count = 0; count < currlinenum; count++) getline(infile2, line2);
        for (int count2 = currlinenum + 1; count2 <= (currlinenum + numofdays); count2++) {
            getline(infile2, line2);
            //cout << count2 << " : " << line << endl;
            double value = atof(line2.substr(11,-1).c_str());
            if (greatest < value) {
                greatest = value;
                greatestdate = line2.substr(0,10);
            }
            if (lowest > value) {
                lowest = value;
                lowestdate = line2.substr(0,10);
            }
        }
        
        int diffindays;
        double green;
        double red;
        
        ss.str("");
        ss << "date --utc --date " << greatestdate << " +%s";
        int greatestsec = atoi(exec(ss.str().c_str()).c_str());
        diffindays = (currentsec - greatestsec) / 86400;
        //cout << diffindays << endl;
        green = numofdays - diffindays;
        green = green / double(numofdays);
        green = green * 100.0;
        
        ss.str("");
        ss << "date --utc --date " << lowestdate << " +%s";
        int lowestsec = atoi(exec(ss.str().c_str()).c_str());
        diffindays = (currentsec - lowestsec) / 86400;
        //cout << diffindays << endl;
        red = numofdays - diffindays;
        red = red / double(numofdays);
        red = red * 100.0;
        
        out.str("");
        cout << "echo " << currentdate << "," << green << "," << red << " >> " << newfile;
        
        getline(infile, line)
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////Gets and sets up variables from GreenRed.sh////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {  //   ./math $1(ticker) $linetotal $numofdays 
    
    ticker = argv[1];                                   //
    file = ticker + "/" + ticker + ".csv";              // Initalizes variables         
    linetotal = atoi(argv[2]);                          // (ticker, file, newfile, linetotal, numofdays,
    numofdays = atoi(argv[3]);                          //  linedivided)
    linedivided = (linetotal - numofdays) / 50;         //////////////////////
    newfile = ticker + "/" + ticker + "GR" + itos(numofdays) + ".csv";      // int to string converter
    //cout << ticker << " " << file << " " << newfile << " " << linetotal << " " << numofdays << endl;
    //string a;
    //getline(cin, a);
    if (check(newfile) == 1) {
        runShort();
    }
    else {
        runFull();
    }
    
    return 0;
}