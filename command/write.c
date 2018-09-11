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

    int fd = open(argv[1],O_RDWR,I_REGULAR);
	if(fd == -1)
    {
        printf("Failed!\n");
        return 0;
    }
    

    int n = write(fd, argv[2],strlen(argv[2]));
	if(n!=strlen(argv[2]))
    {
         printf("Failed!\n");
        return 0;
    }

	printf("write: %s\n", argv[2]);
    close(fd);

	
	return 0;
}
