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
		if(argc != 2)
		{
			printf("Wrong command line !\n");
			return 0;
		}
		else
		{
			int centid;
			int extid[4];
	//		int cbnum;
			int stat;
			int i;
			centp.pri = 2;
			centp.temp = atoi(argv[1]);
			centp.pid = 0;
			centp.stable = 0;
			centid = msgget(centbox, 0600 | IPC_CREAT);
			for(i = 1; i <= 4; i++)
			{
				extid[i - 1] = msgget((centbox + i), 0600 | IPC_CREAT);		
			}
			printf("Start central process, temperature = %d\n", centp.temp);
			int fin = 0;
			while(centp.stable != 1)
			{
				int j;
				int count = 0;
				int total = 0;
				for(j = 1; j <= 4; j++)
				{
					stat = msgrcv(centid, &extp, sizeof(extp) - sizeof(long), 2, 0);
					total = total + extp.temp;

					if(centp.temp == extp.temp)
							count++;
				}
			
				if(count == 4)
				{
					fin++;	
				}
				if(fin == 2)
				{
					centp.stable = 1;
					int l;
					for(l = 1; l <= 4; l++)
					{
						stat = msgsnd(extid[l], &centp, sizeof(centp) - sizeof(long), 0);		
					}
				}
				else
				{
					centp.temp = ((2 * centp.temp + total) / 6);
					int k;
					for(k = 1; k <= 4; k++)
					{
						stat = msgsnd(extid[k], &centp, sizeof(centp) - sizeof(long), 0);		
					}
				}
			}
		printf("Final central temperature = %d\n", centp.temp);
		stat = msgctl(centid, IPC_RMID, 0);
		printf("Close central process %d\n", centp.pid);
		}
		return 0;
}

