#include <iostream>
#include <string>
#include "fetch.h"
#include <fstream>
#include <sys/utsname.h>
#include <ctime>
using namespace std;

void fetchDogBreedArt(const string& breed){
    // Construct the filename based on the breed
        string filename = "dogbreeds/" + breed + ".txt"; // Ensure this path is correct
        ifstream artFile(filename.c_str()); // Open the file

        if (artFile.is_open()) {
            string line;
            cout << "Breed: " << breed << endl; // Display breed name
            while (getline(artFile, line)) {
                cout << line << endl; // Print each line of ASCII art
            }
            artFile.close();
        } else {
            cout << "Could not open file for breed: " << breed << endl; // Error message
        }
}
void fetchSystemInfo() {
    // Fetching the Linux distribution name
    ifstream osFile("/etc/os-release");
    string line, distro;
    if (osFile.is_open()) {
        while (getline(osFile, line)) {
            if (line.find("PRETTY_NAME") != string::npos) {
                distro = line.substr(line.find('=') + 2);
                distro.erase(distro.find_last_of('"')); // Remove trailing quote
                break;
            }
        }
        osFile.close();
    } else {
        cout << "Unable to read OS information." << endl;
    }
    cout << "Distro: " << distro << endl;

    // Fetching CPU information
    ifstream cpuFile("/proc/cpuinfo");
    if (cpuFile.is_open()) {
        while (getline(cpuFile, line)) {
            if (line.find("model name") != string::npos) {
                cout << "CPU: " << line.substr(line.find(':') + 2) << endl; // Extracting CPU model
                break;
            }
        }
        cpuFile.close();
    } else {
        cout << "Unable to read CPU information." << endl;
    }

    long memTotalKB = 0, memFreeKB = 0, memAvailableKB = 0; // Added variable for available memory
        ifstream memFile("/proc/meminfo");
        if (memFile.is_open()) {
            string line;
            while (getline(memFile, line)) {
                if (line.find("MemTotal") != string::npos) {
                    memTotalKB = stol(line.substr(line.find(':') + 2)); // Get total memory in kB
                }
                if (line.find("MemFree") != string::npos) {
                    memFreeKB = stol(line.substr(line.find(':') + 2)); // Get free memory in kB
                }
                if (line.find("MemAvailable") != string::npos) {
                    memAvailableKB = stol(line.substr(line.find(':') + 2)); // Get available memory in kB
                }
                // Break the loop if all values are found
                if (memTotalKB > 0 && memFreeKB > 0 && memAvailableKB > 0) {
                    break;
                }
            }
            memFile.close();
        } else {
            cout << "Unable to read memory information." << endl;
        }

        // Calculate used memory based on total and available
        long memUsedKB = memTotalKB - memAvailableKB;

        // Convert memory values to GB
        double memTotalGB = memTotalKB / 1024.0 / 1024.0; // Convert total memory to GB
        double memUsedGB = memUsedKB / 1024.0 / 1024.0;   // Convert used memory to GB
        double memFreeGB = memFreeKB / 1024.0 / 1024.0;   // Convert free memory to GB
        double availGB = memAvailableKB / 1024.0 / 1024.0;

        cout<<"Memory: "<<availGB<<" / "<<memTotalGB<<endl;

    // Fetching system uptime
    ifstream uptimeFile("/proc/uptime");
    if (uptimeFile.is_open()) {
        double uptimeSeconds;
        uptimeFile >> uptimeSeconds;
        uptimeFile.close();
        int days = uptimeSeconds / 86400; // Convert seconds to days
        int hours = (uptimeSeconds / 3600) - (days * 24);
        int minutes = (uptimeSeconds / 60) - (days * 1440) - (hours * 60);
        cout << "Uptime: " << days << " days, " << hours << " hours, " << minutes << " minutes" << endl;
    } else {
        cout << "Unable to read uptime information." << endl;
    }

    // Fetching Linux kernel version
    struct utsname buffer;
    if (uname(&buffer) == 0) {
        cout << "Linux Kernel: " << buffer.release << endl; // Display the kernel version
    } else {
        cout << "Unable to read kernel information." << endl;
    }
}
