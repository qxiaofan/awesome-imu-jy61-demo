
#include "Uart61.h"

namespace IMU_JY61{
    int uart_open(int fd,const char *pathname)
    {
        fd = open(pathname, O_RDWR|O_NOCTTY);
        if (-1 == fd)
        {
            perror("Can't Open Serial Port");
            return(-1);
        }
        else
            printf("open %s success!\n",pathname);
        if(isatty(STDIN_FILENO)==0)
            printf("standard input is not a terminal device\n");
        else
            printf("isatty success!\n");
        return fd;
    }

    int uart_set(int fd,int nSpeed, int nBits, char nEvent, int nStop)
    {
        struct termios newtio,oldtio;
        if  ( tcgetattr( fd,&oldtio)  !=  0) {
            perror("SetupSerial 1");
            printf("tcgetattr( fd,&oldtio) -> %d\n",tcgetattr( fd,&oldtio));
            return -1;
        }
        bzero( &newtio, sizeof( newtio ) );
        newtio.c_cflag  |=  CLOCAL | CREAD;
        newtio.c_cflag &= ~CSIZE;
        switch( nBits )
        {
            case 7:
                newtio.c_cflag |= CS7;
                break;
            case 8:
                newtio.c_cflag |= CS8;
                break;
        }
        switch( nEvent )
        {
            case 'o':
            case 'O':
                newtio.c_cflag |= PARENB;
                newtio.c_cflag |= PARODD;
                newtio.c_iflag |= (INPCK | ISTRIP);
                break;
            case 'e':
            case 'E':
                newtio.c_iflag |= (INPCK | ISTRIP);
                newtio.c_cflag |= PARENB;
                newtio.c_cflag &= ~PARODD;
                break;
            case 'n':
            case 'N':
                newtio.c_cflag &= ~PARENB;
                break;
            default:
                break;
        }

        /*设置波特率*/

        switch( nSpeed )
        {
            case 2400:
                cfsetispeed(&newtio, B2400);
                cfsetospeed(&newtio, B2400);
                break;
            case 4800:
                cfsetispeed(&newtio, B4800);
                cfsetospeed(&newtio, B4800);
                break;
            case 9600:
                cfsetispeed(&newtio, B9600);
                cfsetospeed(&newtio, B9600);
                break;
            case 115200:
                cfsetispeed(&newtio, B115200);
                cfsetospeed(&newtio, B115200);
                break;
            case 460800:
                cfsetispeed(&newtio, B460800);
                cfsetospeed(&newtio, B460800);
                break;
            default:
                cfsetispeed(&newtio, B9600);
                cfsetospeed(&newtio, B9600);
                break;
        }
        if( nStop == 1 )
            newtio.c_cflag &=  ~CSTOPB;
        else if ( nStop == 2 )
            newtio.c_cflag |=  CSTOPB;
        newtio.c_cc[VTIME]  = 0;
        newtio.c_cc[VMIN] = 0;
        tcflush(fd,TCIFLUSH);

        if((tcsetattr(fd,TCSANOW,&newtio))!=0)
        {
            perror("com set error");
            return -1;
        }
        printf("set done!\n");
        return 0;
    }

    int uart_close(int fd)
    {
        assert(fd);
        close(fd);

        return 0;
    }
    int send_data(int  fd, char *send_buffer,int length)
    {
        length=write(fd,send_buffer,length*sizeof(unsigned char));
        return length;
    }
    int recv_data(int fd, char* recv_buffer,int length)
    {
        length=read(fd,recv_buffer,length);
        return length;
    }
    float a[3],w[3],Angle[3],h[3];
    void ParseData(char chr)
    {
        static char chrBuf[100];
        static unsigned char chrCnt=0;
        signed short sData[4];
        unsigned char i;

        time_t now;
        chrBuf[chrCnt++]=chr;
        if (chrCnt<11) return;

        if ((chrBuf[0]!=0x55)||((chrBuf[1]&0x50)!=0x50)) {printf("Error:%x %x\r\n",chrBuf[0],chrBuf[1]);memcpy(&chrBuf[0],&chrBuf[1],10);chrCnt--;return;}

        memcpy(&sData[0],&chrBuf[2],8);

        switch(chrBuf[1])
        {
            case 0x51:
                for (i=0;i<3;i++) a[i] = (float)sData[i]/32768.0*16.0;
                time(&now);
                printf("\r\nT:%s a:%6.3f %6.3f %6.3f ",asctime(localtime(&now)),a[0],a[1],a[2]);
                break;
            case 0x52:
                for (i=0;i<3;i++)
                    w[i] = (float)sData[i]/32768.0*2000.0;
                printf("w: %7.3f %7.3f %7.3f\n",w[0],w[1],w[2]);
                break;
            case 0x53:
            {
                for (i=0;i<3;i++)
                    Angle[i] = (float)sData[i]/32768.0*180.0;
                printf("A:%7.3f %7.3f %7.3f\n",Angle[0],Angle[1],Angle[2]);
            }
                break;
            case 0x54:
                for (i=0;i<3;i++) h[i] = (float)sData[i];
                printf("h:%4.0f %4.0f %4.0f\n",h[0],h[1],h[2]);
                break;
        }
        chrCnt=0;
    }

    void ParseData(char chr,long time_base)
    {
        static char chrBuf[100];
        static unsigned char chrCnt=0;
        signed short sData[4];
        unsigned char i;

        time_t now;
        chrBuf[chrCnt++]=chr;
        if (chrCnt<11) return;

        if ((chrBuf[0]!=0x55)||((chrBuf[1]&0x50)!=0x50)) {printf("Error:%x %x\r\n",chrBuf[0],chrBuf[1]);memcpy(&chrBuf[0],&chrBuf[1],10);chrCnt--;return;}

        memcpy(&sData[0],&chrBuf[2],8);

//        fflush(stdout);
//        setvbuf(stdout,NULL,_IONBF,0);
//        freopen("../imu_data.txt","a",stdout);

       std::cout.precision(5);

        switch(chrBuf[1])
        {
            case 0x51:
            {
                fflush(stdout);
                setvbuf(stdout,NULL,_IONBF,0);
                freopen("../imu_acc.mat","a",stdout);

                long time = getTimeToBase(time_base);
                float time_dist =float(time)*0.0001;
                std::cout<<time_dist<<" ";
                for (i=0;i<3;i++)
                {
                    //src
                    a[i] = (float)sData[i]/32768.0*16.0;

                    //qxiaofan
                    //a[i] = (float)sData[i] * 16.0;
                }

                //time(&now);
                //printf("\r\nT:%s a:%6.3f %6.3f %6.3f ",asctime(localtime(&now)),a[0],a[1],a[2]);
                printf("%6.3f %6.3f %6.3f\n",a[0],a[1],a[2]);
            }
                break;
            case 0x52:
            {
                fflush(stdout);
                setvbuf(stdout,NULL,_IONBF,0);
                freopen("../imu_gyo.mat","a",stdout);
                long time = getTimeToBase(time_base);
                float time_dist =float(time)*0.0001;
                std::cout<<""<<time_dist<<" ";
                for (i=0;i<3;i++)
                {
                    //src
                    w[i] = (float)sData[i]/32768.0*2000.0;

                    //qxiaofan
                    //w[i] = (float)sData[i]*2000.0;
                }
                printf("%7.3f %7.3f %7.3f\n",w[0],w[1],w[2]);
            }

                break;
            case 0x53:
            {
                fflush(stdout);
                setvbuf(stdout,NULL,_IONBF,0);
                freopen("imu_angle.mat","a",stdout);
                long time = getTimeToBase(time_base);
                float time_dist =float(time)*0.0001;
                std::cout.precision(5);
                std::cout<<time_dist<<" ";
                for (i=0;i<3;i++)
                {
                    //src
                    Angle[i] = (float)sData[i]/32768.0*180.0;

                    //qxiaofan
                    //Angle[i] = (float)sData[i] * 180.0;
                }
                printf("%7.3f %7.3f %7.3f ",Angle[0],Angle[1],Angle[2]);

            }

                break;
            case 0x54:
                for (i=0;i<3;i++) h[i] = (float)sData[i];
                printf("h:%4.0f %4.0f %4.0f ",h[0],h[1],h[2]);

                break;
        }
        chrCnt=0;
    }

    long getTime()
    {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
        //printf("%l\n",time);
    }

    long getTimeToBase(long time_base)
    {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000 - time_base;
        //printf("%l\n",time);
    }

}
