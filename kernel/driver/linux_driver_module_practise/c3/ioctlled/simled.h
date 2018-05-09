
#ifndef _LEDDEV_H_
#define _LEDDEV_H_

#ifndef LED_MAJOR
#define LED_MAJOR 251
#endif

struct led_dev {
    char* data;
    unsigned long size;
};

#endif /* _MEMDEV_H_ */
