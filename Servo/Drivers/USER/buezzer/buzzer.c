#include	"buzzer.h"
#include "tim.h"
#include "usart.h"
#include "stdio.h"
#include "stdarg.h"		//包含需要的头文件 
#include "string.h"		//包含需要的头文件 
unsigned char beat;   //当前节拍索引
unsigned char note;   //当前节拍对应的音符
unsigned int time = 0;      //当前节拍计时
unsigned int beatTime = 0;  //当前节拍总时间
unsigned int soundTime = 0; //当前节拍需发声时间

unsigned char enable = 1;   //蜂鸣器发声使能标志
unsigned char tmrflag = 0;  //定时器中断完成标志



/*原版音调*/
//unsigned int  NoteFrequ[] = {  //中音1-7和高音1-7对应频率列表
//    523,  587,  659,  698,  784,  880,  987,  //中音1-7
//    1047, 1175, 1319, 1397, 1568, 1760, 1976  //高音1-7
//};
///*C调*/
//unsigned int  NoteFrequ[] = {  //中音1-7和高音1-7对应频率列表
//    131, 147, 165, 175, 196, 221, 248,  //低音1-7
//    262, 294, 330, 350, 393, 441, 495, //中音1-7
//		525, 589, 661, 700, 786, 882, 990,		//高音1-7
//};

/*C调调整*/
unsigned int  NoteFrequ[] = {  //中音1-7和高音1-7对应频率列表
    262, 294, 330, 350, 393, 441, 495, //中音1-7
		131, 147, 165, 175, 196, 221, 248,  //低音1-7
		525, 589, 661, 700, 786, 882, 990,		//高音1-7
};


///*D调*/
//unsigned int  NoteFrequ[] = {  //中音1-7和高音1-7对应频率列表
//    147, 165, 175, 196, 221, 248, 278,  //低音1-7
//    294, 330, 350, 393, 441, 495, 556, //中音1-7
//		589, 661, 700, 786, 882, 990, 1112,		//高音1-7
//};

///*D调调整*/
//unsigned int  NoteFrequ[] = {  //中音1-7和高音1-7对应频率列表
//    294, 330, 350, 393, 441, 495, 556, //中音1-7
//		147, 165, 175, 196, 221, 248, 278,  //低音1-7
//		589, 661, 700, 786, 882, 990, 1112,		//高音1-7
//};

///*E调*/
//unsigned int  NoteFrequ[] = {  //中音1-7和高音1-7对应频率列表
//    165, 175, 196, 221, 248, 278, 312,  //低音1-7
//    330, 350, 393, 441, 495, 556, 624, //高音1-7
//		661, 700, 786, 882, 990, 1112, 1248,		//高音1-7
//};





unsigned char Buzzer_play_Enable = 0;//3:播放两遍，比实际数少一遍
/* 两只老虎乐曲播放函数 */
    //两只老虎音符表
unsigned char  TwoTigerNote[] = {
        1,   2,   3, 1,    1,   2,   3, 1,   3, 4, 5,   3, 4, 5,
        5,6, 5,4, 3, 1,    5,6, 5,4, 3, 1,   2 ,5, 1,   2, 5, 1,
    };

    //两只老虎节拍表，4表示一拍，1就是1/4拍，8就是2拍
unsigned char  TwoTigerBeat[] = {
        4,   4,   4, 4,    4,   4,   4, 4,   4, 4, 8,   4, 4, 8,
        3,1, 3,1, 4, 4,    3,1, 3,1, 4, 4,   4, 4, 8,   4, 4, 8,
    };

//测试音符
unsigned char  testNote[] = {
        1,	2,	3,	4,	5,	6,	7,
				7,	6,	5,  4,	3,	2,	1,
    };

//测试节拍
		
unsigned char  testBeat[] = {
        4,   4,   4, 4,    4,   4,   4,
				4,   4,   4, 4,    4,   4,   4,
    };
		
///*小星星*/
//const unsigned char  TwoTigerNote[] = {
//        1,   1,   5, 5,    6,   6,   5,      4, 4, 3, 3,   2, 2, 1,
//        5,   5,   4, 4,    3,   3,   2,      5, 5, 4, 4,   3, 3, 2,	
//        1,   1,   5, 5,    6,   6,   5,      4, 4, 3, 3,   2, 2, 1,
//    };
//    //小星星节拍表，4表示一拍，1就是1/4拍，8就是2拍
//const unsigned char  TwoTigerBeat[] = {
//        4,   4,   4, 4,    4,   4,   8,  		 4, 4, 4, 4,   4, 4, 8,
//        4,   4,   4, 4,    4,   4,   8,  		 4, 4, 4, 4,   4, 4, 8,	
//        4,   4,   4, 4,    4,   4,   8,  		 4, 4, 4, 4,   4, 4, 8,
//    };

		
/////*起床号*/
//unsigned char  TwoTigerNote[] = {
//        5,     1*3,   3*3,  1*3, 
//				3*3,   5*3,   5,    1*3,   5,
//				1*3, 	 3*3,   1*3,  3*3,   5*3,
//    };
///*起床号音符2*/
//unsigned char  TwoTigerNote[] = {
//        5,     1,   3,  1, 
//				3,   5,   5,    1,   5,
//				1, 	 3,   1,  3,   5,

//    };
///*起床号节拍3*/
//unsigned char  TwoTigerBeat[] = {
//        20,     20,   20,  28, 
//				20,   20,   20,    28,   20,
//				20, 	20,   28,  20,   20,

//    };
//  /*起床号节拍4*/
//const unsigned char  TwoTigerBeat[] = {
//        28,     28,   28,  49, 
//				28,   28,   28,    49,   28,
//				28, 	28,   49,  28,   28,

//    };


double countA;
unsigned char *pNote;
unsigned char *pBeat;	
unsigned char Note_1;	
unsigned char _pNote[1],_pBeat[1];

//播放音符1~7，或者
//参数:   _Note：音符   _Beat:节拍 _count：播放次数

void Play_Note(unsigned char _Note, unsigned char _Beat,unsigned char play_count)
{
	if(_Note<=21)//音符高中低音3*7
	{
		_pNote[0] = _Note;
		_pBeat[0] = _Beat;
		pNote = &_pNote[0];
		pBeat = &_pBeat[0];
		Note_1 = 1;
		Buzzer_play_Enable =play_count+1;//2：使能
		beat = 0;
		time = 0;
	}
	else
	{
		if(_Note == 101)
		{
			Note_1 = sizeof(TwoTigerNote);
			pNote = TwoTigerNote;
			pBeat = TwoTigerBeat;
			Buzzer_play_Enable =play_count+1;//2：使能
			beat = 0;
			time = 0;
		}
		if(_Note ==200)
		{
			Note_1 = sizeof(testNote);
			pNote = testNote;
			pBeat = testBeat;
			Buzzer_play_Enable =play_count+1;//2：使能
			beat = 0;
			time = 0;
		}
	}
	HAL_TIM_Base_Start_IT(&htim6);
}



void PlayTwoTiger(void)
{
	if(Buzzer_play_Enable >1)
	{
    if(tmrflag == 1)
		{
			tmrflag = 0;
        if (time == 0)  //当前节拍播完则启动一个新节拍
        {
            note = pNote[beat] - 1;
						countA = 200000000/200;
						countA = countA/(double)(NoteFrequ[note]*2);
//						TIM_SetAutoreload(BASIC_TIM,(uint16_t)countA-1);
						__HAL_TIM_SET_AUTORELOAD(&htim1, (uint16_t)countA-1);
//						TIM_SetCounter(BASIC_TIM, (uint16_t)countA-2);
//						TIM_ITConfig(BASIC_TIM, TIM_IT_Update, DISABLE);/*DISABLE,ENABLE使能计数器中断*/
            beatTime = (pBeat[beat] * NoteFrequ[note]) >> 2;
            soundTime = beatTime - (beatTime >> 2);//计算发声时间，为总时间的0.75，移位原理同上
            enable = 1;  //指示蜂鸣器开始发声
//						TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);/*DISABLE,ENABLE使能计数器中断*/
            time++;
        }
        else  //当前节拍未播完则处理当前节拍
        {
						
            if (time >= beatTime)  //当前持续时间到达节拍总时间时归零，
            {                      //并递增节拍索引，以准备启动新节拍
                time = 0;
                beat++;
//							enable = 0;
            }
            else  //当前持续时间未达到总时间时，
            {
                time++;   //累加时间计数
                if (time == soundTime)  //到达发声时间后，指示关闭蜂鸣器，
                {                       //插入0.25*总时间的静音间隔，
                    enable = 0;         //用以区分连续的两个节拍
                }
            }
        }
				if(beat==Note_1)
				{
					
					beat=0;
//					enable = 0;
					Buzzer_play_Enable-=1;
				}
    }
	}
	else
	{
		if(Buzzer_play_Enable == 1)//关闭
		{
			Buzzer_play_Enable = 0;
			beat = 0;
			BUZZER1(0);//关闭蜂鸣器
			HAL_TIM_Base_Stop_IT(&htim6);
//			TIM_ITConfig(BASIC_TIM, TIM_IT_Update, DISABLE);/*DISABLE,ENABLE使能计数器中断*/
//			delay_ms(200);
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == (&htim1))
	{
		if(Buzzer_play_Enable != 0)
		{
			tmrflag = 1;
			if(enable){BUZZER1TOGGLE;}  //使能时反转蜂鸣器控制电
			else{BUZZER1(0);}         //未使能时关闭蜂鸣器
			PlayTwoTiger();
		}
	}
}


//#define UART4_MACRO   5 
uint8_t UART4_Rx_Buffer;
//uint8_t UART4_Tx_Buffer[36];
//uint8_t UART4_len = 0;


//UART4_len = 0;
//HAL_UART_Transmit_IT(&huart4, UART4_Tx_Buffer, uint16_t Size)

__align(8) char HMI_TxBuff[36];//串口3缓存区

void hmi_printf(char* fmt,...) 
{  
	unsigned int length;
	
	va_list ap;
	
	//va_list 可变参数列表，存参数地址
	va_start(ap,fmt);//获取可变参数地址 fmt地址赋给ap
	
	vsprintf(HMI_TxBuff,fmt,ap);/*使用参数列表发送格式化输出到字符串,
	函数功能将可变参数格式化输出到一个字符数组
	*/
	//fmt中内容赋给Usart3_TxBuff，
	va_end(ap);	//清空参数列表
	//
	length=strlen((const char*)HMI_TxBuff);		
//	while(RESET == usart_flag_get(HMI_USART,USART_FLAG_TBE)); // 等待发送数据缓冲区标志置位
//	for(i = 0;i < length;i ++)
//	{			
		HAL_UART_Transmit(&huart4,(uint8_t*)HMI_TxBuff,length,0xffff);
//		while(RESET == usart_flag_get(HMI_USART,USART_FLAG_TBE)); // 等待发送数据缓冲区标志置位
//	}	
}

extern uint8_t runing_state;

char rx_buffer[5];
int rx_index;
//串口屏中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint32_t tick;
	static uint32_t rx_tick = 0;
	
	tick = HAL_GetTick();
	
	if (tick - rx_tick > 10)
	{
		rx_index = 0;
	}
	rx_tick = tick;
	if(huart==&huart4)
	{
		if (rx_index >= 5)
		{
			 rx_index = 0;
		}
		rx_buffer[rx_index++] = UART4_Rx_Buffer;
		
		if(rx_index == 5 && rx_buffer[0] == 0X7E && rx_buffer[3]==0XFF && rx_buffer[4]==0XFF)
		{
			printf("0X%X",rx_buffer[1]);
			runing_state = rx_buffer[1];
		}
		
		HAL_UART_Receive_IT(&huart4,&UART4_Rx_Buffer,UART4_MACRO);
	}
	
}








/*********************************************END OF FILE**********************/
