/*

Strace:

1.	mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f6e22d8c000
mmap(NULL, length, 可被讀取|可被寫入, copy on write|忽略-1，不涉及文件)
在開啟檔案時確認內容是否有開啟成功。
Sourse = fopen(“argv[1]”,  “r”);
2. open("/etc/ld.so.cache", O_RDONLY)      = 3
   連同1一起開檔。
3.fstat(3, {st_mode=S_IFREG|0644, st_size=30761, ...}) = 0
聚集檔案的統計資料，傳回一陣列，包含inode、連結數目、user ID, user qroup ID, types, size, 最後存取、修改、變更時間, blocksize……
4.read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0 \24\2\0\0\0\0\0"..., 832) = 832
　　從指定的檔案（3）中讀取832個字節到”\177ELF…”開始的緩衝區。
5. write(1, "Wrong command !\n", 16Wrong command !
)       = 16
　　會把參數buf內所指的內存寫入16個字節至所指的文件（1）內

Ltrace:

1.__libc_start_main(0x4006a4, 1, 0x7fffeb2bc2d8, 0x4007b0, 0x400840 <unfinished ...>
2.puts("Wrong command !"Wrong command !
)                                                     = 16
3.exit(0 <unfinished ...>
+++ exited (status 0) +++


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define max 100001

int main(int argc, char *argv[])
{
	if (argc < 3 || argc > 5)
	{
		printf("Wrong command !\n");
		exit(0);
	}
		
	FILE *sourse, *target;

	sourse = fopen(argv[1], "r");

	if(sourse == NULL)
	{
		printf("File no exist !\n");
		exit(EXIT_FAILURE);
	}

	target = fopen(argv[2], "w");

	if (target == NULL)
	{
		fclose(sourse);
		printf("Press any key to exit...");
		exit(EXIT_FAILURE);
	}

	char ch;

	while((ch = fgetc(sourse)) != EOF)
			fputc(ch, target);

	fclose(sourse);
	fclose(target);

	return 0;
}
