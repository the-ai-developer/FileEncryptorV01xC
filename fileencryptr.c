#include "fileencryptr.h"
#include <fcntl.h>
#include <sys/random.h>

int8 *securerand(int16 size){
    int8 *ptr,*start;
    size_t n;

    assert(size > 0);
    ptr = (int8 *)malloc(size);
    assert(ptr);
    start = ptr;
    
    n = getrandom(ptr, (size_t)size, GRND_RANDOM|GRND_NONBLOCK);

    if (size == n){
        return ptr;
    }else if(n < 0){
        free(ptr);
        return 0;
    }

    fprintf(stderr, "Warning: Entropy Pool Is Empty! This May Take Longer Than Usual!\n");
    ptr+=n;
    n = getrandom(ptr, size-n , GRND_RANDOM|GRND_NONBLOCK);
    
    if(n == size){
        return start;
    }else{
        free(start);
        return 0;
    }
}

int8 *readkey(char *prmpt){
    char DataX[256];
    int8 *ptr;
    int8 SizeX,Idx;

    printf("%s ",prmpt);
    fflush(stdout);

    toggleEcho(false);
    memset(DataX,0,256);
    read(0, DataX, 255);

    SizeX = (int8)strlen(DataX);
    Idx = SizeX - 1;
    ptr = (int8 *)DataX + Idx;
    *ptr = 0;

    ptr = (int8 *)malloc(SizeX);
    assert(ptr);
    strncpy((char *)ptr,DataX, Idx);
    toggleEcho(true);

    return ptr;
}

void toggleEcho(bool IsEnabled){
    struct termios *t;

    t = (struct termios *)malloc(sizeof(struct termios));

    tcgetattr(0, t);
    if(IsEnabled){
        t->c_cflag |= ECHO;
    }else{
        t->c_cflag &= ECHO;
    }
    tcsetattr(0,0,t);
    return ;
}

int main(int argc, char *argv[]){
    Arcfour *rc4;
    char *InpFile,*OutFile;
    int infd,outfd;
    int8 *key;
    int16 keySize;
    int16 padSize;
    int8 *padSize8;
    int16 *padSize16;
    int8 *paddByte;

    if(argc < 3){
        fprintf(stderr,"Usage: %s <inputFile> <outputFile>\n",*argv);
        return -1;
    }

    InpFile = argv[1];
    OutFile = argv[2];

    infd = open(InpFile, O_RDONLY);
    if (infd < 1){
        perror("open");
        return -1;
    }

    outfd = open(OutFile, O_WRONLY|O_CREAT,00600);
    if (outfd < 1){
        close(infd);
        perror("open");
        return -1;
    }

    key = readkey("Key: ");
    assert(key);
    keySize = (int16)strlen((char *)key);
    padSize8 = securerand(2);
    padSize16 = (int16 *)padSize8;
    padSize = *padSize16;
    printf("PadSize: %d\n",(int)padSize); 

    close(infd);
    close(outfd);
    free(padSize8);

    return 0;
}
