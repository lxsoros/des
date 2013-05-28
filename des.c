#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static const unsigned char ip[64]={
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17, 9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7
};

static const unsigned char reverse_ip[64]={
	40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41, 9,49,17,57,25
};

static const unsigned char extension_permutation[48]={
	32, 1, 2, 3, 4, 5,
	 4, 5, 6, 7, 8, 9,
	 8, 9,10,11,12,13,
	12,13,14,15,16,17,
	16,17,18,19,20,21,
	20,21,22,23,24,25,
	24,25,26,27,28,29,
	28,29,30,31,32, 1
};

static const unsigned char data_permutation[32]={
	16, 7,20,21,29,12,28,17,
	 1,15,23,26, 5,18,31,10,
	 2, 8,24,14,32,27, 3, 9,
	19,13,30, 6,22,11, 4,25
};

static const unsigned char s[][64]={
{
	14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7,
	 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8,
	 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0,
	15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13
},
{
	15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10,
	 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5,
	 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15,
	13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9
},
{
	10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8,
	13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1,
	13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7,
	 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12
},
{
	 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15,
	13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9,
	10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4,
	 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14
},
{
	 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9,
	14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6,
	 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14,
	11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3
},
{
	12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11,
	10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8,
	 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6,
	 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13
},
{
	 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1,
	13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6,
	 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2,
	 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12
},
{
	13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7,
	 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2,
	 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8,
	 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11
}
};

static const unsigned char key_pc1[56]={
	57,49,41,33,25,17, 9,
	 1,58,50,42,34,26,18,
	20, 2,59,51,43,35,27,
	19,11, 3,60,52,44,36,
	63,55,47,39,31,23,15,
	 7,62,54,46,38,30,22,
	14, 6,61,53,45,37,29,
	21,13, 5,28,20,12, 4
};

static const unsigned char key_pc2[48]={
	14,17,11,24, 1, 5, 3,28,
	15, 6,21,10,23,19,12, 4,
	26, 8,16, 7,27,20,13, 2,
	41,52,31,37,47,55,30,40,
	51,45,33,48,44,49,39,56,
	34,53,46,42,50,36,29,32
};

static const unsigned char shift_num[16]={
	1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};

/*-----------function define----------------*/
void shift_key(unsigned char *const,const unsigned char *,int);
int hex_to_bin(unsigned char *const,unsigned char *const,int);
int permutation(unsigned char *const,unsigned char *const,const unsigned char *,int,int);
void shift_left(unsigned char *const);
void xor(unsigned char *const,unsigned char *const,unsigned char *const,int);
void s_replace(unsigned char *const,unsigned char *const,const unsigned char (*)[]);
void print_array(unsigned char *const,int,int);
/*-----------function define----------------*/
int main(int argc,char *args[])
{
	char *data;
	char *key;
	int err_flag=0;
	unsigned char data_bin[64],key_bin[64],key_bin_nocrc[56];
	unsigned char *const data_bin_left=data_bin,*data_bin_right=&data_bin[32];
	unsigned char data_cache[32],data_cache1[32],data_cache_48[48],key_48[48];
	unsigned char s_temp[32],xor_temp[48];
	if(argc<=2)
		printf("This is a DES demo Program.\nAuthor:wengpingbo\nEmail:wengpingbo@gmail.com\nUsage:\n\tdes [16bit hex encipher data] [16bit hex key]\nNotice:if specify two more argument,the rest will be ignored\n");
	else if(argc>2)
	{
		data=args[1];
		key=args[2];
		for(int i=0;i<16;i++)
		{
			if(i!=1 && (data[i]=='\0' || key[i]=='\0')) {err_flag=3;break;} //check the length of data and key
			else if(!isxdigit(data[i])) {err_flag=1;break;} //check if is hex
			else if(!isxdigit(key[i])) {err_flag=2;break;} //check if is hex
			else data[i]=toupper(data[i]);
		}
		if(!err_flag)
		{
			hex_to_bin(data,data_bin,16);
			hex_to_bin(key,key_bin,16);
			print_array(data_bin,64,0);
			exit(0);
			permutation(data_bin,data_bin,ip,64,64);
			for(int i=0,k=0;i<64;i++) //shrink the key from 64 to 56
				if((i+1)%8!=0) key_bin_nocrc[k++]=key_bin[i];
			permutation(key_bin_nocrc,key_bin_nocrc,key_pc1,56,56);
			for(int k=0;k<16;k++)
			{
				for(int i=0;i<32;i++)
					data_cache[i]=data_bin_right[i];
				permutation(data_bin_right,data_cache_48,extension_permutation,32,48);
				shift_key(key_bin_nocrc,shift_num,k);
				permutation(key_bin_nocrc,key_48,key_pc2,56,48);
				xor(data_cache_48,xor_temp,key_48,48);
				s_replace(xor_temp,s_temp,s);
				permutation(s_temp,data_cache1,data_permutation,32,32);
				xor(data_bin_left,data_cache1,data_bin_right,32);
				for(int i=0;i<32;i++)
				{
					if(k!=16)
					{
						data_bin_right[i]=data_cache1[i];
						data_bin_left[i]=data_cache[i];
					}
					else
					{
						data_bin_right[i]=data_cache[i];
						data_bin_left[i]=data_cache1[i];
					}
				}
			}
			permutation(data_bin,data_bin,reverse_ip,64,64);
			fprintf(stdout,"%x\n",data_bin);
		}
	}
	switch(err_flag)
	{
		case 1:
			fprintf(stderr,"Error:The data \"%s\" is not a hex string!\n",data);
			break;
		case 2:
			fprintf(stderr,"Error:The key \"%s\" is not a hex string!\n",key);
			break;
		case 3:
			fprintf(stderr,"Error:The length of input data or key is ilegal!\n");
			break;
	}
	return err_flag;

}


void print_array(unsigned char *const arg,int length,int type)
{
	for(int i=0;i<length;i++)
	{
		if(type==0)
			fprintf(stdout,"%d ",arg[i]);
		else if(type==1)
			fprintf(stdout,"%o ",arg[i]);
		else if(type==2)
			fprintf(stdout,"%x ",arg[i]);
		else if(type==3)
			fprintf(stdout,"%s ",arg[i]);
		if((i+1)%10==0) fprintf(stdout,"\n");
	}
}
int hex_to_bin(unsigned char *const hex,unsigned char *const bin,int length)
{
	for(int i=0;i<length*4;i++)
		*(bin+i)=0;
	for(int i=0;i<length;i++)
	{
		unsigned char temp=hex[i];
		if(temp>=65) temp-=55;
		else temp-=48;
		unsigned char org=temp;
		for(int k=i*4+3;k>=i*4;k--)
		{
			temp>>=1;
			if(temp!=0) bin[k]=org-temp*2;
			else {bin[k]=temp;break;}
		}
	}
	return 0;
}


int permutation(unsigned char *const arg,unsigned char *const output,const unsigned char * table,int a_len,int o_len)
{
	unsigned char temp[a_len];
	for(int i=0;i<a_len;i++)
		temp[i]=arg[i];
	for(int i=0;i<o_len;i++)
		output[i]=temp[table[i]-1];
	return 0;
}

void shift_left(unsigned char *const key_28)
{
	unsigned char temp;
	temp=key_28[0];
	for(int i=0;i<27;i++)
		key_28[i]=key_28[i+1];
	key_28[27]=temp;
}

void shift_key(unsigned char *const key_56,const unsigned char *table,int count)
{
	unsigned char *key_left=key_56;
	unsigned char *key_right=&key_56[28];
	for(int i=0;i<table[count];i++)
	{
		shift_left(key_left);
		shift_left(key_right);
	}
}

void xor(unsigned char *const data,unsigned char *const output,unsigned char *const key,int length)
{
	for(int i=0;i<length;i++)
		output[i]=data[i]^key[i];
}

void s_replace(unsigned char *const data,unsigned char *const output,const unsigned char table[][64])
{
	int row,col,result;
	for(int i=0;i<8;i++)
	{
		row=data[i*6]*2+data[i*6+5];
		col=data[i*6+1]*2*2*2+data[i*6+2]*2*2+data[i*6+3]*2+data[i*6+4];
		result=table[i][row*16+col];
		unsigned char org=result;
		for(int k=i*4+3;k>=i*4;k--) //convert hex to binary
		{
			result>>=1;
			if(result!=0) output[k]=org-result*2;
			else {output[k]=result;break;}
		}
	}
}
