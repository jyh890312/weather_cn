// wether.c
//
// Copyright (C) 2013 - jyh
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//           wether.c
//  Sun April 07 08:53:26 2013
//  Copyright  2013  jyh
//  <user@host>

#include "wether.h"

#define MAX_LEN 1024;

/***************************************************************************
　　* FUNCTION: gbk2utf8
　　* DESCRIPTION: 实现由gbk编码到utf8编码的转换
　　*
　　* Input: utfStr,转换后的字符串; srcStr,待转换的字符串; maxUtfStrlen, utfStr的最
　　大长度
　　* Output: utfStr
　　* Returns: -1,fail;>0,success
　　*
　　* modification history
　　* --------------------
　　* 2011-nov-25, lvhongya written
　　* --------------------
　*************************************************************************/
int gbk2utf8(char *utfStr,char *srcStr,int maxUtfStrlen){
	if(NULL==srcStr){
		printf("Bad Parameter\n");
		return -1;
	}
	if(NULL==setlocale(LC_ALL,"zh_CN.gbk")){
		printf("Bad Parameter\n");
		return -1;
	}
	int unicodeLen=mbstowcs(NULL,srcStr,0);//计算转换后的长度
	if(unicodeLen<=0){
		printf("Can not Transfer!!!\n");
		return -1;
	}
	wchar_t *unicodeStr=(wchar_t *)calloc(sizeof(wchar_t),unicodeLen+1);
	mbstowcs(unicodeStr,srcStr,strlen(srcStr));//将gbk转换为unicode
	if(NULL==setlocale(LC_ALL,"zh_CN.utf8")){
		printf("Bad Parameter\n");
		return -1;
	}
	int utfLen=wcstombs(NULL,unicodeStr,0);//计算转换后的长度
	if(utfLen<=0)
	{
		printf("Can not Transfer!!!\n");
		return -1;
	}else if(utfLen>=maxUtfStrlen){
		printf("Dst Str memory not enough\n");
		return -1;
	}
	wcstombs(utfStr,unicodeStr,utfLen);
	utfStr[utfLen]=0;//添加结束符
	free(unicodeStr);
	return utfLen;
}

bool getWebtoXmlFile(char *url){
	int sockfd;
	struct sockaddr_in addr;
	struct hostent *pURL;
	static char header[256];
	static char text[1024];

	/* Create a TCP socket */
	sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	pURL = gethostbyname(url);
	if (NULL == pURL){
		return false;
	}
	/* Construct the addr for use */
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	addr.sin_addr= *(struct in_addr *)*pURL->h_addr_list;
	/*Construct a TCP connection*/
	if(-1 == connect(sockfd,(struct sockaddr *)&addr,sizeof(addr))){ 
		printf("connect error!"); 
		return false;
	}
	
	//string sCode = getCityCode(m_sCity);
	memset(header,'\0',sizeof(header));
	strcat(header,"GET ");
	strcat(header,"/WebServices/WeatherWebService.asmx/getWeatherbyCityName?theCityName=");	
	strcat(header,"54511");
	strcat(header," HTTP/1.1\r\n");
	strcat(header,"HOST: ");
	strcat(header,"www.webxml.com.cn");
	strcat(header,"\r\nConnection: Close\r\n\r\n");
	/* Send the package */
	send(sockfd,header,strlen(header),0);

	char tmp[10*1024];
	memset(tmp,'\0',sizeof(tmp));
	while (recv(sockfd,text,sizeof(text),0) != 0){
		strcat(tmp,text);
	}
	/*disconnect the TCP connection*/
	close(sockfd);
	/*write to a xml file*/
	char utf8_tmp[10*1024];
	gbk2utf8(utf8_tmp,tmp,10*1024);
	writetofile("./xmlWether",tmp,"w+");
	//puts(tmp);
	return true;
}

void writetofile(char *path,char *s,char *mode){
	/*write to a xml file*/
	FILE *pXmlFile;
	pXmlFile = fopen(path,mode);	//rewrite
	if(NULL != pXmlFile){
		fwrite(s,sizeof(char),strlen(s),pXmlFile);
	}
	else{
		perror("error to write xml file!");
		fclose(pXmlFile);
		return ;
	}
	//fcolse(pXmlFile);//why undefined reference to 'fclose'?!
	return;
}

void readfromfile(char *path,char *s){
	/*read from a xml file*/
	FILE *pXmlFile;
	long len;
	size_t result;
	
	pXmlFile = fopen(path,"rb");	
	if(NULL != pXmlFile){
		fseek(pXmlFile,0,SEEK_END);
		len = ftell(pXmlFile);
		rewind(pXmlFile);
		result = fread(s,sizeof(char),len,pXmlFile);
		//fgets(s,len,pXmlFile);
		if(result != len){
			perror("error to read xml file!");
			return;
		}
	}
	else{
		perror("error to write xml file!");
		fclose(pXmlFile);
		return ;
	}
	fclose(pXmlFile);//why undefined reference to 'fclose'?!
	return;
}

void paraseXmlWetherInfo(char info[][255],int *type){
	char xmlfiletmp[10*1024];
	char *pch_start,*pch_end;
	int str_len;
	char infotmp[255];

	readfromfile("./xmlWether",xmlfiletmp);

	int i;
	for(i=0;i<11;i++){
		pch_start = strstr(xmlfiletmp,"<string>");
		strncpy(pch_start,"00000000",8);
		pch_start += 8;
		pch_end = strstr(xmlfiletmp,"</string>");
		strncpy(pch_end,"000000000",9);
		str_len = pch_end-pch_start;
		//memset(infotmp,'\0',50);
		strncpy(infotmp,pch_start,str_len);
		infotmp[str_len] = '\0';
		//memset(info+i+str_len,'\0',1);
		//strncpy(info1+str_len,"\0",1);
		//printf("%s\n",infotmp);
		strcpy(&info[i],infotmp);
		//strcat(info,"\n");
		if(8 == i){
			char typeStr[2] = {'\0','\0'};
			typeStr[0] = infotmp[0];
			*type = atol(typeStr);
		}
	}
	return;
}
