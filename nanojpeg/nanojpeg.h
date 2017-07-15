#ifndef _NANOJPEG_H_
#define _NANOJPEG_H_

void njInit(void);

void njDone(void);

typedef enum _nj_result {
    NJ_OK = 0,        // no error, decoding successful
    NJ_NO_JPEG,       // not a JPEG file
    NJ_UNSUPPORTED,   // unsupported format
    NJ_OUT_OF_MEM,    // out of memory
    NJ_INTERNAL_ERR,  // internal error
    NJ_SYNTAX_ERROR,  // syntax error
    __NJ_FINISHED,    // used internally, will never be reported
} nj_result_t;

nj_result_t njDecode(const void* jpeg, const int size);

int njGetWidth(void);
int njGetHeight(void);
unsigned char* njGetImage(void);
int njGetImageSize(void);
#endif
