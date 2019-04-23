#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>

const int centbox = 1024;

struct{
	long pri;
	int temp;
	int pid;
	int stable;
} extp, centp;

int main(int argc, char *argv[])
{
		if(argc != 3)
		{
			printf("Wrong command line !\n");
			return 0;
		}
		else
		{
			int centid;
			int extid;
			int ebnum;
			int stat;
			int pid = atoi(argv[2]);

			extp.pri = 2;
			extp.temp = atoi(argv[1]);
			extp.pid = pid;
			extp.stable = 0;

			ebnum = centbox + pid;

			centid = msgget(centbox, 0600 | IPC_CREAT);
			extid = msgget(ebnum, 0600 | IPC_CREAT);		
			printf("Start external process %d, temperature = %d\n", extp.pid, extp.temp);

			while(centp.stable != 1)
			{
					stat = msgsnd(centid, &extp, sizeof(extp) - sizeof(long), 0);
					stat = msgrcv(extid, &extp, sizeof(centp) - sizeof(long), 2, 0);

					if(centp.temp != 1)
					{
						extp.temp = (extp.temp * 3 + 2 * centp.temp) / 5;		
					}
							
				printf("Final external temperature = %d\n", extp.temp);
				stat = msgctl(extid, IPC_RMID, 0);
				printf("Close external process %d\n", extp.pid);
			}
		}
		return 0;
}

