#include "type.h"
#include "stdio.h"
#include "sys/const.h"
#include "sys/protect.h"
#include "string.h"
#include "sys/fs.h"
#include "sys/proc.h"
#include "sys/tty.h"
#include "sys/console.h"
#include "sys/global.h"
#include "sys/keyboard.h"
#include "sys/proto.h"

int main(int argc, char * argv[])
{
    int fd=0;
	fd=unlink(argv[1]);
    if(fd!=0)
    {
        printf("Failed!\n");
        return 0;
    }
    printf("remove: %s\n",argv[1]);
	return 0;
}
