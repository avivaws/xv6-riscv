#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    printf("Hello World xv6\n");
    int cd = channel_create();
    if (cd < 0) {
        printf("Failed to create channel\n");
        exit(1);
    }
    if (fork() == 0) {
        if (channel_put(cd, 42) < 0) {
            printf("Failed to put data in channel\n");
            exit(1);
        }
        if(channel_put(cd, 43)==-1) {// Sleeps until cleared
            printf("Failed to put data in channel #1\n");
            exit(1);
        }
        sleep(100); // Sleep until parent destroys channel
        if(channel_destroy(cd)==-1){
            printf("Failed to destroy channel\n");
            exit(1);
        }
    } else {
        int data;
        if (channel_take(cd, &data) < 0) { // 42
        printf("Failed to take data from channel\n");
        exit(1);
        }else{
            printf("Data received from channel: %d\n", data);
        }
        if(channel_take(cd, &data)==-1){
            printf("Failed to take data from channel #2\n");
            exit(1);
        }else{
            printf("Data received from channel: %d\n", data);// 43
        }
        data = channel_take(cd, &data); // Sleep until child destroys channel
        if(data==-1){
            printf("Failed to take data from channel #3 - this is good\n");
            exit(1);
        }else{
            printf("Data received from channel: %d\n", data);
        }
    }

    exit(0);
}
