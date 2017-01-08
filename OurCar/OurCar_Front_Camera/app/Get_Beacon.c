#include "common.h"
#include "servo.h"
#include "motor.h"
#include "Get_Beacon.h"

extern uint8_t pic[60][80];//解压后的图像数据
extern uint8_t H_Mid_line_flag;//后摄像头信标接收标志
extern  uint8_t F_Mid_line_flag;

long int abs_long_int(long int dat)
{
	return dat>0?dat:-dat;
}

//图像二值化图像做异或操作并将异或结果保存在img0中
//img0 img1  ： 两幅二值化图像，即一个bit表示一个像素
//Pixelcnt ： 图像的字节数
void img_XOR(uint8_t * img0,uint8_t * img1,uint16_t Pixelcnt)
{
	uint16_t i=0;
	for(i=0;i<Pixelcnt;i++)
	{
		*(img0+i)^=*(img1+i);
	}
}

/// 回溯法标记连通域
//x :          该点的横坐标
//y :          该点的纵坐标
//isMarked :   该点是否已经被标记过，用于记录回溯路线。未标记为0，如果该点已经被标记过，则应指定该参数为1
//由于条件判断太多，所以可能写的有点晕，也许有些情况是不可能发生的，但还是做了条件判断
//！！！还是有Bug，但是效果满足要求！！！

#define q 60                     //不同连通域灰度值差距，这只是为了把标记的连通域发回上位机查看标记效果
uint8_t mark=0;                     //被标记的个数，即连通域标记
uint8_t mat[60][80]={0};            //暂时把标记连通域图像放在这里，其实没必要单独开一块空间
extern uint8_t pic[60][80];				//解压后二值化图像，1 byte--->1 pixel 
uint8_t Back_Flag=0;                //回溯标志，用在了记录连通域数目时，对mark的减操作

float X_Start = 11;//图像起始行，主要是为了避免光线的干扰（图像上方是干扰重灾区）

void Connect(int x, int y, uint8_t isMarked)
{
	if (x == (uint8_t)X_Start && y == 0) //mat[0, 0]
	{
		//第一次进入复位mark标记数和回溯标志
		mark=0;
		Back_Flag=0;
		if (pic[x][y] != 0 )
		{			
			mat[x][y] = (++mark)*q+1; // 新的连通域
		}
	}else if (x != (uint8_t)X_Start && y == 0) // 
	{
		if (pic[x][y] != 0)
		{
			if (mat[x - 1][ y] != 0)
			{
				if(isMarked ==0)
				{
					mat[x][y] = mat[x - 1][y]; 
				}else
				{
					if(mat[x-1][y] > mat[x][y])
					{
						mat[x-1][y] = mat[x][y]; // up one
						Connect(x - 1, y, 1);
					}
				}
			}
			else
			{
				if (isMarked == 0)
				{
					if(Back_Flag==1)   //未被标记且之前回溯过，说明上一轮回溯完成，对标记数mark做减一操作
					{
						mark--;
						Back_Flag=0;
					}
					mat[x][y] = (++mark)*q+1; // 新的连通域
				}
			}
		}
	}else if (x == (uint8_t)X_Start && y != 0) // First Column
	{
		if (pic[x][y] != 0)
		{
			if (mat[x][ y-1] != 0)
			{
				if(isMarked == 0)
				{
					mat[x][y] = mat[x][ y-1]; 
				}else
				{
					if(mat[x][ y-1]>mat[x][ y])
					{
						mat[x][y-1] = mat[x][ y]; // left one
						Connect(x, y - 1, 1);	
					}
				}
			}
			else
			{
				if (isMarked == 0)
				{
					if(Back_Flag==1)
					{
						mark--;
						Back_Flag=0;
					}
					mat[x][y] = (++mark)*q+1; // 新的连通域
				}
			}
		}
	}else if (x != (uint8_t)X_Start && y != 0) // other pixel
	{
		if (pic[x][y] != 0)
		{
			if (mat[x][ y-1] == 0 && mat[x - 1][ y] == 0)
			{
				if (isMarked == 0)
				{
					if(Back_Flag==1)
					{
						mark--;
						Back_Flag=0;
					}
					mat[x][y] = (++mark)*q+1;   // 新的连通域
				}
			}
			else if (mat[x][ y-1] == 0 && mat[x - 1][ y] != 0)
			{
				if (isMarked == 0)
				{
					if(Back_Flag==1)
					{
						mark--;
						Back_Flag=0;
					}
					mat[x][y] = mat[x - 1][y];
				}
				else
				{
					if (mat[x - 1][ y] > mat[x][ y])
					{
					mat[x - 1][ y] = mat[x ][y];
					Connect(x - 1, y, 1); // 沿x方向继续回溯
					}
				}
			}
			else if (mat[x][ y-1] != 0 && mat[x - 1][ y] == 0)
			{
				if (isMarked == 0)
				{
					if(Back_Flag==1)
					{
						mark--;
						Back_Flag=0;
					}
					mat[x][y] = mat[x][ y-1];
				}
				else
				{
					if (mat[x][ y-1] > mat[x][ y])
					{
					mat[x][ y-1] = mat[x ][y];
					Connect(x, y - 1, 1); // 沿y方向继续回溯
					}
				}
			}
			else if (mat[x][ y-1] != 0 && mat[x - 1][ y] != 0 && mat[x][ y-1] == mat[x - 1][y])
			{
				if (isMarked == 0)
				{
					if(Back_Flag==1)
					{
						mark--;
						Back_Flag=0;
					}
					mat[x][y] = mat[x][ y-1];
				}
				else
				{
					if (mat[x][ y-1] > mat[x][ y])
					{
							mat[x][ y-1] = mat[x - 1][ y] = mat[x ][y];
							Connect(x - 1, y, 1); // 遇到上边和左边都有已标记像素的情况，两边同时回溯
							Connect(x, y - 1, 1);
					}
				}
			}
			else if (mat[x][ y-1] != 0 && mat[x - 1][ y] != 0 && mat[x][ y-1] != mat[x - 1][y])
			{
				if(isMarked == 0)
				{
					if(Back_Flag==1)
					{
						mark--;
						Back_Flag=0;
					}
					if(mat[x - 1][ y]>mat[x][ y - 1])
					{
						mat[x - 1][ y] = mat[x][y] = mat[x][ y - 1];
						Connect(x - 1, y, 1);
						if(mat[x-1][ y]!=mat[x-1][ y-1])
						Back_Flag=1;
					}else if(mat[x - 1][ y]<mat[x][ y - 1])
					{
						mat[x][ y-1] = mat[x][y] = mat[x-1][ y];
						Connect(x, y - 1, 1);
						if(mat[x-1][ y]!=mat[x-1][ y-1])
						Back_Flag=1;
					}
	  		}else
				{
					if(mat[x - 1][ y]>mat[x][y])
					{
						mat[x - 1][ y] = mat[x][y];
						Connect(x - 1, y, 1);
					}
					if(mat[x ][ y-1] > mat[x][y])
					{
						mat[x ][ y-1] = mat[x][y];
						Connect(x , y-1, 1);
					}
				}
			}
		}
	}					
}
		
#define x1 0       //连通域标记，X起点
#define x2 1       //连通域标记，X终点
#define y1 2       //连通域标记，Y起点
#define y2 3       //连通域标记，Y终点

//Aspect ratio						
float Ar_max=1.5;//1.33;         //宽高比最大值
float Ar_min=0.5;//0.77;         //宽高比最小值
//White ratio
float Wr_min = 0.5;//0.7; //白色区域占矩形的比例最小值   0.785;pi/4
 
#define Connected_domain_cnt 20
uint8_t Connected_domain[Connected_domain_cnt][4]={0};//连通域特征值记录
uint16_t White_cnt[Connected_domain_cnt] = {0};
float White_Ratio=0.0;   //白色区域占矩形的比例
float W[5]={0.0};        //连通域宽
float H[5]={0.0};        //连通域高
float WH_Ratio=0.0; //连通域宽高比
float White_Min = 3;//图像在阈值比较大的时候出现一些白色噪点，限制白色连通域最小白色像素点个数滤除噪点干扰。
void GetBeacon(uint8_t Mat[60][80],struct Beacon * DstBeacon)
{
	F_Mid_line_flag=0;
	int16 xx=0;
	int16 yy=0;
	uint16_t White_cnt_max=White_Min;
  for(int16 xx=(uint8_t)X_Start;xx<60;xx++)
 {
		for(int16 yy=0;yy<80;yy++)
		{
			 mat[xx][yy]=0;
		}
	}
	for(xx=(uint8_t)X_Start;xx<60;xx++)
  {
	  for(yy=0;yy<80;yy++)
	  {
		  Connect(xx,yy,0);
  	}
  }
	//连通域标记、白色区域计数复位
	for(xx=0;xx<Connected_domain_cnt;xx++)
	{
		Connected_domain[xx][x1] = 255;
		Connected_domain[xx][x2] = 0;
		Connected_domain[xx][y1] = 255;
		Connected_domain[xx][y2] = 0;
		White_cnt[xx] = 0;
	}
	//连通域过多，认为是干扰，退出，不标记
	if(mark>=Connected_domain_cnt)
		return;
	//标记连通域，目前是遍历，有待改进
	for(xx=(uint8_t)X_Start;xx<60;xx++)
	{
		for(yy=0;yy<80;yy++)
		{
			 if(Mat[xx][yy]!=0)
			 {
				 White_cnt[(Mat[xx][yy]-1)/q]++;
				 if(xx< Connected_domain[(Mat[xx][yy]-1)/q][x1])
				 {
					 Connected_domain[(Mat[xx][yy]-1)/q][x1] = xx;
				 }
				 if(xx> Connected_domain[(Mat[xx][yy]-1)/q][x2])
				 {
					 Connected_domain[(Mat[xx][yy]-1)/q][x2] = xx;
				 }
				 if(yy< Connected_domain[(Mat[xx][yy]-1)/q][y1])
				 {
					 Connected_domain[(Mat[xx][yy]-1)/q][y1] = yy;
				 }
				 if(yy> Connected_domain[(Mat[xx][yy]-1)/q][y2])
				 {
					 Connected_domain[(Mat[xx][yy]-1)/q][y2] = yy;
				 }
			 }
		 }
	 }
	//计算连通域宽高比，
	for(int16 marknum=1;marknum<=mark;marknum++)
	{
		 H[marknum-1] =  (float)(Connected_domain[marknum][x2]-Connected_domain[marknum][x1])+1;
		 W[marknum-1] =  (float)(Connected_domain[marknum][y2]-Connected_domain[marknum][y1])+1;
		 WH_Ratio=W[marknum-1]/H[marknum-1];
		 White_Ratio = (float)White_cnt[marknum]/(H[marknum-1]*W[marknum-1]);
		 if(WH_Ratio<Ar_max&& WH_Ratio>Ar_min && White_Ratio>Wr_min)
		 {
			 //取白点最多的连通域为信标，有待改进
			 if(White_cnt[marknum]>White_cnt_max&&White_cnt[marknum]>5)
			 {
				 White_cnt_max = White_cnt[marknum];
				 DstBeacon->x = (Connected_domain[marknum][x2]+Connected_domain[marknum][x1])/2;
				 DstBeacon->y = (Connected_domain[marknum][y2]+Connected_domain[marknum][y1])/2;
				 F_Mid_line_flag=1;
			 }
			 uint8_t temp = marknum*q+1;
			 //又是遍历，有待改进
			 for(xx=(uint8_t)X_Start;xx<60;xx++)
				{
					for(yy=0;yy<80;yy++)
					{
						 if(Mat[xx][yy]==temp)
						 {
							 Mat[xx][yy]=255;
						 }
					 }
				 }
		 }
	 }     
}			


uint8_t Beacon_cnt = 0;//记录第几个信标
uint8_t Pre_Beacon_cnt = 0;
uint8_t Beaco_cnt_flag = 1;
float Cnt_X_Thre = 30;
float Cnt_Max=3.0;
extern struct Beacon DstBeacon;//目标信标位置
/*********************************************
* 信标计数更新
*********************************************/
void Beacon_CNT_Update(void)
{
	Beacon_cnt=Pre_Beacon_cnt;
}
/*********************************************
* 信标计数
*********************************************/
void Beacon_CNT(void)
{
	if(DstBeacon.x>Cnt_X_Thre)
	{
		if(Beaco_cnt_flag>(uint8_t)Cnt_Max)
		{
			Pre_Beacon_cnt++;
		}
		Beaco_cnt_flag=0;	
	}else
	{
		Beaco_cnt_flag++;
		if(Beaco_cnt_flag>(uint8_t)Cnt_Max)
		{
			Beaco_cnt_flag=(uint8_t)(Cnt_Max+1);
			Beacon_CNT_Update();
		}
	}
}





					



