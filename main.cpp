
#include "utils.h"
#include "Uart61.h"
using namespace IMU_JY61;

int main(void)
{
    //qxiaofan added
    //自动识别串口
    list<string> l = getComList();

    list<string>::iterator it = l.begin();

    std::string serial_port= "";
    std::string b = "USB";
    string::size_type idx;

    int success_num = 0;

    while (it != l.end()) {
        cout << *it << endl;

        idx = it->find(b);
        if(idx != string::npos)
        {
            cout<<"found\n";
            success_num++;
            serial_port = *it;
        }
        it++;
    }

    if(success_num == 1)
    {
        std::cout<<"find serial port, OK!!!"<<std::endl;
        std::cout<<"serial_port == "<<serial_port<<std::endl;
    }

    if(success_num > 1)
    {
        std::cout<<"find more than one serial port,please choose check!!!"<<std::endl;
    }

    if(success_num < 1)
    {
        std::cout<<"not found right serial port,please check!!!"<<std::endl;
    }
    //qxiaofan ended

    char r_buf[1024];
    bzero(r_buf,1024);

    //src
    //fd = uart_open(fd,"/dev/ttyUSB1");/*串口号/dev/ttySn,USB口号/dev/ttyUSBn */

    fd = uart_open(fd,serial_port.c_str());

    if(fd == -1)
    {
        fprintf(stderr,"uart_open error\n");
        exit(EXIT_FAILURE);
    }

    if(uart_set(fd,BAUD,8,'N',1) == -1)
    {
        fprintf(stderr,"uart set failed!\n");
        exit(EXIT_FAILURE);
    }

    //src
	//FILE *fp;
	//fp = fopen("Record.txt","w");

	long time_base = getTime();
    while(1)
    {
        ret = recv_data(fd,r_buf,44);
        if(ret == -1)
        {
            fprintf(stderr,"uart read failed!\n");
            exit(EXIT_FAILURE);
        }
		for (int i=0;i<ret;i++) {
		    //fprintf(fp,"%2X ",r_buf[i]);
		    //std::cout<<"r_buf["<<i<<"] == "<<r_buf[i]<<std::endl;
		    ParseData(r_buf[i]);
		}
        usleep(1000);
    }

    ret = uart_close(fd);
    if(ret == -1)
    {
        fprintf(stderr,"uart_close error\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
