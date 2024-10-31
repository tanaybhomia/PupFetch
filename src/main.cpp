// the main file for calling both the functions
#include <iostream>
#include <cstdio>
#include <string>
#include "fetch.h"
#include "config.h"
#include "dogbreeds.h"
using namespace std;

int main(int argc,char* argv[]){

    if(argc<2){
        cerr <<"Usage: "<<argv[0]<<"[breed]"<<endl;
    }

    string breed = argv[1];
    displayDogArt(breed);
    fetchSystemInfo();
    return 0;
}
