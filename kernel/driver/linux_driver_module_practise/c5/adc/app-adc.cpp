#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char **argv) 
{ 
        int fd; 
        float vo; 
        int t0; 
  
        fd = open("/dev/mini2440_adc", 0); 
 
        if(fd < 0) 
        { 
                printf("Open ADC Device Faild!\n"); 
                exit(1);         } 
 
        while(1) 
        { 
                int ret; 
                int data; 
                 

                ret = read(fd, &data, sizeof(data)); 
            
                vo = ((float)data*3.3)/1024.0; 
                t0 = (vo - (int)vo)*1000; 
                if(ret != sizeof(data)) 
                { 
                        if(errno != EAGAIN) 
                        { 
                                printf("Read ADC Device Faild!\n"); 
                        } 
 
                        continue; 
                } 
                else 
                { 
                        printf("Read ADC volty is: %d.%-3dV\n", (int)vo,t0); 
                        sleep(2); 
                } 
        } 
 
        close(fd); 
 
        return 0; 
} 
