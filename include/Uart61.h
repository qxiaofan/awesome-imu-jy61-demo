#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<assert.h>
#include<termios.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<sys/types.h>
#include<errno.h>

#include<iostream>

#define BAUD 115200 //115200 for JY61 ,9600 for others

namespace IMU_JY61{

    static int ret;
    static int fd;

    int uart_open(int fd,const char *pathname);

    int uart_set(int fd,int nSpeed, int nBits, char nEvent, int nStop);

    int uart_close(int fd);

    int send_data(int  fd, char *send_buffer,int length);

    int recv_data(int fd, char* recv_buffer,int length);

    void ParseData(char chr);

    void ParseData(char chr,long time);

    long getTime();

    long getTimeToBase(long time_base);
}

