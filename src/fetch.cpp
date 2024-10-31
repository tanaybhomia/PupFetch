#include <iostream>
#include <string>
#include "fetch.h"
#include <algorithm>
#include <fstream>
#include <sys/utsname.h>
#include <unordered_map>
#include <ctime>
using namespace std;

string cpuicon = "";
string ramicon = "";
string uptimeicon = "";
string kernelicon = "";

string fetchDogBreedArt(const string& breed) {
    // Access the art directory in the project root
    string filename = "../art/" + breed;
    if (filename.find(".txt") == string::npos) {
        filename += ".txt";  // Add ".txt" extension if missing
    }
    ifstream artFile(filename.c_str());
    string result;

    if (artFile.is_open()) {
        string line;
        while (getline(artFile, line)) {
            result += line + "\n";
        }
        artFile.close();
    } else {
        result = "Could not open file for breed: " + breed;
    }
    return result;
}
string fetchDistroIcon(const string& distroName) {
    // Map of distro icons using Nerd Font Unicode characters
    unordered_map<string, string> distroIcons = {
        {"Fedora", ""},  // Fedora icon
        {"Ubuntu", ""},  // Ubuntu icon
        {"Arch Linux", "󰣇"}, // Arch Linux icon
        {"Debian", ""},  // Debian icon
        {"Manjaro", ""}, // Manjaro icon
        {"Red Hat", "󰮤"}, // Red Hat icon
        {"Kali Linux", ""}, // Kali Linux icon (Debian-based)
        {"openSUSE", ""}  // openSUSE icon
    };

    // Iterate over the map to find a matching distro
        for (const auto& pair : distroIcons) {
            if (distroName.find(pair.first) != string::npos) {
                return pair.second; // Return the icon for the matching distro
            }
        }
        return "\uf17c";  // Default Linux icon if no matches found
}

// Function to fetch the Linux distribution name
string fetchDistro() {
    ifstream osFile("/etc/os-release");
    string line, distro;
    if (osFile.is_open()) {
        while (getline(osFile, line)) {
            if (line.find("PRETTY_NAME") != string::npos) {
                distro = line.substr(line.find('=') + 2);
                distro.erase(distro.find_last_of('"'));
                break;
            }
        }
        osFile.close();
    } else {
        distro = "Unable to read OS information.";
    }
    string icon = fetchDistroIcon(distro);
    return icon+"   "+distro;
}

// Function to fetch CPU model information
string fetchCPUInfo() {
    ifstream cpuFile("/proc/cpuinfo");
    string cpuModel = "Unknown";
    string line;
    if (cpuFile.is_open()) {
        while (getline(cpuFile, line)) {
            if (line.find("model name") != string::npos) {
                cpuModel = line.substr(line.find(':') + 2);
                break;
            }
        }
        cpuFile.close();
    } else {
        cpuModel = "Unable to read CPU information.";
    }
    return cpuicon+ "   " + cpuModel;
}

// Function to fetch memory information
string fetchMemoryInfo() {
    long memTotalKB = 0, memAvailableKB = 0;
    ifstream memFile("/proc/meminfo");
    string line;
    if (memFile.is_open()) {
        while (getline(memFile, line)) {
            if (line.find("MemTotal") != string::npos) {
                memTotalKB = stol(line.substr(line.find(':') + 2));
            } else if (line.find("MemAvailable") != string::npos) {
                memAvailableKB = stol(line.substr(line.find(':') + 2));
                break;
            }
        }
        memFile.close();
    }
    double memTotalGB = memTotalKB / 1024.0 / 1024.0;
    double availGB = memAvailableKB / 1024.0 / 1024.0;
    return ramicon +"   "+ to_string(availGB) + " GB / " + to_string(memTotalGB) + " GB";
}

// Function to fetch system uptime
string fetchUptime() {
    ifstream uptimeFile("/proc/uptime");
    string result;
    if (uptimeFile.is_open()) {
        double uptimeSeconds;
        uptimeFile >> uptimeSeconds;
        uptimeFile.close();
        int days = uptimeSeconds / 86400;
        int hours = (uptimeSeconds / 3600) - (days * 24);
        int minutes = (uptimeSeconds / 60) - (days * 1440) - (hours * 60);
        result = uptimeicon +"   "+ to_string(days) + " days, " + to_string(hours) + " hours, " + to_string(minutes) + " minutes";
    } else {
        result = "Unable to read uptime information.";
    }
    return result;
}

// Function to fetch Linux kernel version
string fetchKernelVersion() {
    struct utsname buffer;
    string kernel;
    if (uname(&buffer) == 0) {
        kernel = kernelicon+"   " + string(buffer.release);
    } else {
        kernel = "Unable to read kernel information.";
    }
    return kernel;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Please specify a dog breed as a command-line argument." << endl;
        return 1;
    }

    string breed = argv[1];

    // Fetch and directly print each piece of information
    cout << fetchDogBreedArt(breed) << endl;
    cout << fetchDistro() << endl;
    cout << fetchCPUInfo() << endl;
    cout << fetchMemoryInfo() << endl;
    cout << fetchUptime() << endl;
    cout << fetchKernelVersion() << endl;

    return 0;
}
