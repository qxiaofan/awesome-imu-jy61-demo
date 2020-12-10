#pragma once
//for std
#include "fstream"
#include "iostream"
#include "sstream"
#include "string"

#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <list>

using namespace std;

//for UWB串口自动识别
string get_driver(const string& tty);

void register_comport( list<string>& comList, list<string>& comList8250, const string& dir);

void probe_serial8250_comports(list<string>& comList, list<string> comList8250);

list<string> getComList();




