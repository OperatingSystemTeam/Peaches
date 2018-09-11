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

	open(".ls",O_RDWR,I_REGULAR);
	//printf("%s",buf);
	return 0;
}
