#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

string exec(const char* cmd) {                  //Gives the ability to set a variable as ouput of system command
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

int main(int argc, char* argv[]) {  //   ./math $file $newfile $linetotal $numofdays 
    string file = argv[1];
    string newfile = argv[2];
    int linetotal = atoi(argv[3]);
    int numofdays = atoi(argv[4]);
    int linedivided = (linetotal - numofdays) / 50;
        //cout << file << " " << newfile << " " << linetotal << " " << numofdays << endl;
    
    for (int currlinenum = 1; currlinenum <= (linetotal - numofdays); currlinenum++) {      //Loop for lines
        string currentdate;
        stringstream ss;
        ifstream infile(file.c_str());
        
        string line;
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
            
        int diffinsec;
        int diffindays;
        double green;
        double red;
        
        ss.str("");
        ss << "date --utc --date " << greatestdate << " +%s";
        int greatestsec = atoi(exec(ss.str().c_str()).c_str());
        diffinsec = currentsec - greatestsec;
        diffindays = diffinsec / 86400;
            //cout << diffindays << endl;
        green = numofdays - diffindays;
        green = green / double(numofdays);
        green = green * 100.0;
        
        ss.str("");
        ss << "date --utc --date " << lowestdate << " +%s";
        int lowestsec = atoi(exec(ss.str().c_str()).c_str());
        diffinsec = currentsec - lowestsec;
        diffindays = diffinsec / 86400;
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