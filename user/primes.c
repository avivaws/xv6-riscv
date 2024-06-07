#include "kernel/types.h"
#include "user/user.h"

int cd1;
int cd2;

void checker(int id){
    int num;
    printf("checker %d started\n",id);
    while(channel_take(cd1,&num)==0){
        int prime=1;
        for(int i=2;i<=num/2;i++){
            if(num%i==0){
                prime=0;
                i=num;
            }
        }
        if(prime==1){
            channel_put(cd2,num);
        }
        
    }
}
void generator(){
    int num=2;
    while(channel_put(cd1,num)==0){
        num++;
    }
}
void printer(){
    int count=0;
    int num;
    printf("printer started\n");
    while(channel_take(cd2,&num)==0){
        count++;
        printf("prime %d is number:%d\n",num,count);
        if(count==100){
            printf("distroying channels\n");
            channel_destroy(cd1);
            channel_destroy(cd2);
        }
    }
    printf("printer finished not cool\n");
}


int
main(int argc, char *argv[])
{
    int checkers=3;
    if(argc>1){
        checkers=atoi(argv[1]);
        if(checkers<1){
            printf("Invalid number of checkers: %d\n",checkers);
            exit(1);
        }
    }
    int running=1;
    while(running){
        printf("Creating Channels\n");
        cd1=channel_create();
        cd2=channel_create();
        printf("cd1=%d cd2=%d\n",cd1,cd2);
        if (cd1 < 0 || cd2 < 0) {
            printf("Failed to create channel\n");
            exit(1);
        }
        for(int i=1;i<=checkers;i++){
            printf("Creating Checker %d\n",i);
            int id=fork();
            if(id<0){
                printf("Failed to create checker %d\n",i);
                exit(1);
            }
            if(id==0){
                checker(i);
                sleep(i+2);
                printf("checker process %d is exiting\n",i);
                exit(1);
            }
        }
        printf("Creating Printer\n");
        int id=fork();
        if(id<0){
            printf("Failed to create printer\n");
            exit(1);
        }
        if(id==0){
            printf("printer process: id = %d\n", id);
            printer();
            sleep(1);
            printf("printer process is exiting\n");
            exit(1);
            
        }
        generator();
        while(wait(0)!=-1);
        printf("generator finished\n");
        char buffer[128];
        do{
        printf("do you want to go again? (y/n)\n");
        read(0,buffer,128);
        }while(buffer[0]!='y'&&buffer[0]!='n');
        if(buffer[0]=='n'){
            running=0;
        }
    }
    return 1;
}

    