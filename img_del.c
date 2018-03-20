#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

int main(int argc, char * argv[])
{
	FILE * in, * out;
	int i, j, ROW, COL;
	float ave;
	char ch;
	printf("Please enter the number of rows and cols\n");
	scanf("%d %d", &ROW, &COL);
	int data[ROW][COL+1];
	if (argc != 2)
		{
			printf("Usage: %s filename\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	if ((in = fopen(argv[1], "r")) == NULL)
		{
			printf("Can not open %s", argv[1]);
			exit(EXIT_FAILURE);
		}
	for (i = 0; i < ROW; i++)
		{
			for (j = 0; j < COL+1; j++)
				{
					if (j == COL)
						{
							data[i][j] = '\0';
						}
					else
						{
							while ((ch = getc(in)) != EOF)
								{
									if (isdigit(ch))
										{
											data[i][j] = ch;
											break;
										}
									else
										{
											continue;
										}
								}
						}
				}
		}
	for (i = 1; i < ROW - 1; i++)               //处理内层图像失真
		{
			for (j = 1; j < COL - 1; j++)
				{
					if (abs(data[i-1][j] - data[i][j]) > 1 && abs(data[i+1][j] - data[i][j]) > 1 && abs(data[i][j-1] - data[i][j]) > 1 && abs(data[i][j+1] - data[i][j]) > 1)
						{
							ave = (data[i-1][j] + data[i+1][j] + data[i][j-1] + data[i][j+1]) / 4;
							data[i][j] = round(ave);
						}
				}
		}
	for (i = 0, j = 1; j < COL - 1; j++)        //处理顶部图像失真
		{
			if (abs(data[i][j-1] - data[i][j]) > 1 && abs(data[i][j+1] - data[i][j]) > 1 && abs(data[i+1][j] - data[i][j]) > 1)
				{
					ave = (data[i][j-1] + data[i][j+1] + data[i+1][j]) / 3;
					data[i][j] = round(ave);
				}
		}
	for (i = ROW - 1, j = 1; j < COL - 1; j++)  //处理底部图像失真
		{
			if (abs(data[i][j-1] - data[i][j]) > 1 && abs(data[i][j+1] - data[i][j]) > 1 && abs(data[i-1][j] - data[i][j]) > 1)
				{
					ave = (data[i][j-1] + data[i][j+1] + data[i-1][j]) / 3;
					data[i][j] = round(ave);
				}
		}
	for (i = 1, j = 0; i < ROW - 1; i++)        //处理左侧图像失真
		{
			if (abs(data[i-1][j] - data[i][j]) > 1 && abs(data[i+1][j] - data[i][j]) > 1 && abs(data[i][j+1] - data[i][j]) > 1)
				{
					ave = (data[i-1][j] + data[i+1][j] + data[i][j+1]) / 3;
					data[i][j] = round(ave);
				}
		}
	for (i = 1, j = COL - 1; i < ROW - 1; i++)  //处理右侧图像失真
		{
			if (abs(data[i-1][j] - data[i][j]) > 1 && abs(data[i+1][j] - data[i][j]) > 1 && abs(data[i][j-1] - data[i][j]) > 1)
				{
					ave = (data[i-1][j] + data[i+1][j] + data[i][j-1]) / 3;
					data[i][j] = round(ave);
				}
		}
	if (abs(data[1][0] - data[0][0]) > 1 && abs(data[0][1] - data[0][0]) > 1)                                           //处理左上顶点图像失真
		{
			ave = (data[1][0] + data[0][1]) / 2;
			data[0][0] = round(ave);
		}
	if (abs(data[1][COL-1] - data[0][COL-1]) > 1 && abs(data[0][COL-2] - data[0][COL-1]) > 1)                           //处理右上顶点图像失真
		{
			ave = (data[1][COL-1] + data[0][COL-2]) / 2;
			data[0][COL-1] = round(ave);
		}
	if (abs(data[ROW-1][1] - data[ROW-1][0]) > 1 && abs(data[ROW-2][0] - data[ROW-1][0]) > 1)                           //处理左下顶点图像失真
		{
			ave = (data[ROW-1][1] + data[ROW-2][0]) / 2;
			data[ROW-1][0] = round(ave);
		}
	if (abs(data[ROW-1][COL-2] - data[ROW-1][COL-1]) > 1 && abs(data[ROW-2][COL-1] - data[ROW-1][COL-1]) > 1)           //处理右下顶点图像失真
		{
			ave = (data[ROW-1][COL-2] + data[ROW-2][COL-1]) / 2;
			data[ROW-1][COL-1] = round(ave);
		}
	if ((out = fopen("image_transfer", "w")) == NULL)
		{
			printf("Can not create output file\n");
			exit(EXIT_FAILURE);
		}
	for (i = 0; i < ROW; i++)
		{
			for (j = 0; j < COL+1; j++)
				{
					switch (data[i][j])
						{
							case '0':
								putc(' ', out);
								break;
							case '1':
								putc('.', out);
								break;
							case '2':
								putc('`', out);
								break;
							case '3':
								putc(':', out);
								break;
							case '4':
								putc('~', out);
								break;
							case '5':
								putc('*', out);
								break;
							case '6':
								putc('=', out);
								break;
							case '7':
								putc('!', out);
								break;
							case '8':
								putc('%', out);
								break;
							case '9':
								putc('#', out);
								break;
							case '\0':
								putc('\n', out);
								break;
							default:
								putc(' ', out);
						}
				}
		}
	if (fclose(in) != 0 || fclose(out) != 0)
		{
			printf("close file fail.\n");
		}

	return 0;
}
