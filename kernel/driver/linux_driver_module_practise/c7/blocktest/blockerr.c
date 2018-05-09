#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>     /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/timer.h>
#include <linux/types.h>    /* size_t */
#include <linux/fcntl.h>    /* O_ACCMODE */
#include <linux/hdreg.h>    /* HDIO_GETGEO */
#include <linux/kdev_t.h>
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>  /* invalidate_bdev */
#include <linux/bio.h>




static struct gendisk* simp_blkdev_disk;

static struct request_queue* simp_blkdev_queue;

#define SIMP_BLKDEV_DEVICEMAJOR COMPAQ_SMART2_MAJOR

#define SIMP_BLKDEV_DISKNAME "simp_blkdev"

#define SIMP_BLKDEV_BYTES (16*1024*1024)

unsigned char* simp_blkdev_data;

struct block_device_operations simp_blkdev_fops = {
    .owner = THIS_MODULE,
};



/*
函数使用elv_next_request()遍历struct request_queue *q中使用struct request *req表
示的每一段，首先判断这个请求是否超过了我们的块设备的最大容量，
然后根据请求的方向rq_data_dir(req)进行相应的请求处理。由于我们使用的是指简单的数组，因此
请求处理仅仅是2条memcpy。
*/
static void simp_blkdev_do_request(struct request_queue* q) {
    struct request* req;

    while ((req = blk_fetch_request(q)) != NULL) {
        if ((blk_rq_pos(req) + blk_rq_cur_sectors(req)) << 9  > SIMP_BLKDEV_BYTES) {
            printk(KERN_ERR SIMP_BLKDEV_DISKNAME": bad request: block=%llu, count=%u\n",
                   (unsigned long long)blk_rq_pos(req), blk_rq_cur_sectors(req)); //req->sector:请求的开始磁道,request.current_nr_sectors：请求磁道数

            blk_end_request_all(req, 0);
            //            end_request(req, 0);//结束一个请求，第2个参数表示请求处理结果，成功时设定为1，失败时设置为0或
            //者错误号。
            continue;
        }

        switch (rq_data_dir(req)) {
        case READ:
            memcpy(req->buffer, simp_blkdev_data + (blk_rq_pos(req) << 9), blk_rq_cur_sectors(req) << 9); //把块设备中的数据装
            //入req->buffer
            blk_end_request_all(req, 1);
            //                end_request(req, 1);
            break;

        case WRITE:
            memcpy(simp_blkdev_data + (blk_rq_pos(req) << 9), req->buffer, blk_rq_cur_sectors(req) << 9); //把req->buffer中的数据写入块设备
            //                end_request(req, 1);
            blk_end_request_all(req, 1);
            break;

        default:
            /* No default because rq_data_dir(req) is 1 bit */
            break;
        }
    }
}

static int __init simp_blkdev_init(void) {
    int ret;

    simp_blkdev_data = vmalloc(SIMP_BLKDEV_BYTES);


    simp_blkdev_queue = blk_init_queue(simp_blkdev_do_request, NULL);

    if (!simp_blkdev_queue) {
        ret = -ENOMEM;
        goto err_init_queue;
    }



    simp_blkdev_disk = alloc_disk(1);

    if (!simp_blkdev_disk) {
        ret = -ENOMEM;
        goto err_alloc_disk;
    }

    strcpy(simp_blkdev_disk->disk_name, SIMP_BLKDEV_DISKNAME);
    simp_blkdev_disk->major =  SIMP_BLKDEV_DEVICEMAJOR;
    simp_blkdev_disk->first_minor = 0;
    simp_blkdev_disk->fops =  &simp_blkdev_fops;
    simp_blkdev_disk->queue =  simp_blkdev_queue;
    set_capacity(simp_blkdev_disk, SIMP_BLKDEV_BYTES >> 9);



    add_disk(simp_blkdev_disk);
    return 0;

err_init_queue:
    blk_cleanup_queue(simp_blkdev_queue);
err_alloc_disk:
    return ret;
}


static void __exit simp_blkdev_exit(void) {
    put_disk(simp_blkdev_disk);
    del_gendisk(simp_blkdev_disk);
    blk_cleanup_queue(simp_blkdev_queue);
    vfree(simp_blkdev_data);
}





module_init(simp_blkdev_init);
module_exit(simp_blkdev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("lxlong");
