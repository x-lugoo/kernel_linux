
#ifndef _POLL_H_
#define _POLL_H_

struct poll_dev {
    bool   flag;
    wait_queue_head_t wq;
    struct timer_list timer;
};

#endif
