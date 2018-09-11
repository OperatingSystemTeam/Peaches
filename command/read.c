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
    char bufr[80];
    int fd = open(argv[1],O_RDWR,I_REGULAR);
	if(fd == -1)
    {
        printf("Failed!\n");
        return 0;
    }
    

    int n = read(fd, bufr,79);
	if(n!=79)
    {
         printf("Failed!\n");
        return 0;
    }
	bufr[n] = 0;
	printf("read: %s\n", bufr);
    close(fd);

	
	return 0;
}
