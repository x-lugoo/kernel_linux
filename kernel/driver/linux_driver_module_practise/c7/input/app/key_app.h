
#ifndef DSP_H
#define DSP_H

typedef struct sysInfo {
    unsigned int* DSP1IMM;
    unsigned int* DSP2IMM;
    int fd_dsp;
    int fd_local;

} sysInfo;



typedef enum {
    AC490_NOT_ON_LINE,
    AC490_ON_LINE,
} AC490Result;

typedef enum {
    DSP1_NOT_ON_LINE,
    DSP2_NOT_ON_LINE,
    DSP_ON_LINE,
    DSP1_MCBSP_FAIL,
    DSP2_MCBSP_FAIL,
    DSP_MCBSP_OK,
    DSP_MCBSP_ALL_FAIL,
} DSPResult;

typedef struct AC490Struct {
    long addr;
    int result;
} AC490Struct;

#define IOCTL_CMD_LOCALBUS_DSP_DOWNLOAD 0x04
#define IOCTL_CMD_RESET_DSP   0x05

#endif
