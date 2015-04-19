#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <cstring>
#include <string>
using namespace std;

void lsCom() {

}
bool isDependant = 0;
int main() {
    string str;
    pid_t pid;
    bool hasArg = 0, term = 0;
    int strSize = 0;
    int pos, arrPos;

    char *arg;
    char *comm;
    int n = 0;
    while(1) {
        cout << "$ ";
        getline(cin, str);
        string sarg, scomm;
        pos = 0;
        strSize = str.size();
        while(pos < strSize) {
            arg = (char*) malloc(256);
            comm = (char*) malloc(256);
            char* arr[3]; 
            string sarg, scomm;
            bool logOR = false, logAND = false;

            arrPos = 0;
            hasArg = 0;
            term = false;
            hasArg = false;

            if(str.at(pos) == ' ') {
                while(pos < strSize && str.at(pos) == ' ') {
                    ++pos;
                }
                if(pos >= strSize) {
                    break;
                }
            }
            while(pos < strSize && str.at(pos) != ' ') {
                if(str.at(pos) == ';') {
                    term = true;
                    break;
                }
                else if(str.at(pos) == '|') {
                    if(pos+1 < strSize && str.at(pos+1) == '|') {
                        logOR = true;
                        break;
                    }
                }
                else if(str.at(pos) == '&') {
                    if(pos+1 < strSize && str.at(pos+1) == '&') {
                        logAND = true;
                        break;
                    }
                }
                else {
                    scomm += str.at(pos);
                    ++pos;
                }
            }

            strcpy(comm,scomm.c_str());
            ++pos; // This is to ommit the whitespace character following the command
            arr[arrPos] = comm;
            ++arrPos;

            while(pos < strSize && !(term) && !(logOR) && !(logAND)) {
               if(str.at(pos) == ';') {
                   ++pos;
                   break;
               }
               else {
                   sarg += str.at(pos);
                   ++pos;
                   hasArg = 1;
               }
            }

            if(hasArg) {
                strcpy(arg, sarg.c_str());
                arr[arrPos] = arg;
                ++arrPos;
            }
            arr[arrPos] = NULL;
            
            /*for(int i = 0; i < 1; ++i) {
                cout << arr[i] << endl;
            }*/
            pid = fork();
            if(pid == 0) {
                if(execvp(arr[0], arr) == -1) {
                    perror("The command could not be executed!");
                    errno = 0;
                }       
            }
            else {
                wait(0);
            }

            free(comm);
            free(arg);
        }
    }

return 0;
}