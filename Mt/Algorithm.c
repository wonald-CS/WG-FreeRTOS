#include "Algorithm.h"
#include "SysTick.h"

const unsigned short wCRCTalbeAbs[] =
{
	0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401, 0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400,
};


/*******************************************************************************************
*@description:CRCת��
*@param[in]��*ptr:���� �� len:���ݳ���
*@return�����ش�������ת����CRCֵ
*@others��
********************************************************************************************/
unsigned short Algorithm_crc16(unsigned char *ptr, unsigned int len) 
{
	unsigned short wCRC = 0xFFFF;
	unsigned short i;
	unsigned char chChar;
	unsigned char temp[2];
	for (i = 0; i < len; i++)
	{
		chChar = *ptr++;
		wCRC = wCRCTalbeAbs[(chChar ^ wCRC) & 15] ^ (wCRC >> 4);
		wCRC = wCRCTalbeAbs[((chChar >> 4) ^ wCRC) & 15] ^ (wCRC >> 4);
	}
	temp[0] = wCRC&0xFF; 
	temp[1] = (wCRC>>8)&0xFF;
	wCRC = (temp[0]<<8)|temp[1];
	return wCRC;
}



//��ȡATָ�����Ϣ�ĳ���
static unsigned int GetAtRspStrLen(unsigned char *p)
{
	unsigned int len;
	len=0;
	while(*p!=0)
	{
		len++;
		if(len>50)
		{
			break;
		}
		p++;
	}
	return len;
}



/*******************************************************************************************
*@description:�����Ƿ����ַ�����ͬ
*@param[in]��*pdata����������
*@return������ͬ�ַ������򷵻��ַ�����ͬ����ʼ�±ꡣ���ޣ�����0xff
*@others��
********************************************************************************************/
unsigned int SeekSrting(unsigned char *str1,unsigned char *str2,unsigned int st1long)
{
	unsigned int i,log,relog,zi;
	unsigned int st2len;
	st2len = GetAtRspStrLen(str2);
	if(st1long >= st2len)
	{
		log = st1long - st2len+1;////�Ƚϵ��ܵĴ��� 
	}
	else{
		return 0xFF;
	}
		
	relog = 0;     //������¼ �ȽϹ����� �ж��ٸ���ĸ��ͬ�� 
	for(i=0;i<log;i++)
	{
		if(*str1 == *str2)
		{
			zi = 1;
			while(zi)
			{
				relog ++;
				if(relog == st2len)
				{
					return i;		//�ַ�����ͬ����ʼ�±�
				}	
				str1++;
				str2++;				 
				if(*str1 == *str2)
				{
				}
				else{
					str1-=relog;
					str2-=relog;
					relog = 0;
					zi = 0;
					break;
				}
				
			}
		}
		str1++;
	}
	return 0xFF;
}
