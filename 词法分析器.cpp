#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<stdio.h> 

using namespace std;

static char reserveWords[32][20] = {
	"auto","break","case","char","const","continue",
	"default","do","double","else","enum","extern",
	"float","for","goto","if","int","long","register",
	"return ","short","signed","sizeof","static","struct",
	"switch","typedef","union","unsigned","void","volatile",
	"while"
};

static char operatorOrDelimiter[36][10] = {
	"+","-","*","/","<","<=",">",">=","=","==",
   "!=",";","(",")","^",",","\"","\'","#","&",
   "&&","|","||","%","~","<<",">>","[","]","{",
   "}","\\",".","\?",":","!"
};

//标识符 
static char IdenticalTbl[1000][50] = {""};

//查找保留字 若查找成功 返回识别码 否则返回-1 即为标识符 
int searchReserve(char reserveWords[][20], char s[]){
	int i;
	for(i = 0; i < 32; i++){
		if(strcmp(reserveWords[i],s) == 0){
			return i + 1;
		}
	}
	return -1; 
} 

//是否是字母
bool IsLetter(char letter){
	if(letter >= 'a' && letter <= 'z' || letter >= 'A' && letter <= 'Z'
			|| letter == '_'){
		return true;
	}
	else{
		return false;
	}
} 

//是否为数字 
bool IsDigit(char digit){
	if(digit >= '0' && digit <= '9'){
		return true;
	}
	else{
		return false;
	}
} 

//去除无用字符和注释
void filterResource(char r[],int pProject){
	char tempString[10000];
	int count = 0;
	int i;
	for(i = 0; i <= pProject; i++){
		//是否为单行注释 
		if(r[i] == '/' && r[i+1] == '/'){
			while(r[i] != '\n'){
				i++;
			}
		}
		
		//是否为多行注释
		if(r[i] == '/' && r[i+1] == '*'){
			i += 2;
			while(r[i] != '*' || r[i+1] != '/'){
				i++;
				if(r[i] == '$'){
					cout<<"错误101:没有找到注释结束符\n"<<endl;
					exit(0);
				}
			}
			//跳过 */ 
			i += 2;
		}
		
		//是否为无用字符 
		if(r[i] != '\n' && r[i] != '\t' && r[i] != '\v' && r[i] != '\r'){
				tempString[count++] = r[i];
			} 
		
		}
		
		tempString[count] = '\0';
		strcpy(r,tempString);
	
}

//扫描器 根据DFA设计 
void Scanner(int &syn, char resourceProject[],char token[],int &pProject){
	//count 用作token[]的指示器,收集有效字符 
	int i,count = 0;
	char ch;
	
	ch = resourceProject[pProject];
	while(ch == ' '){
		pProject++;
		ch = resourceProject[pProject];
	}
	
	for(i = 0; i < 20; i++){
		token[i] = '\0';
	}
	
	//字母开头 
	if(IsLetter(resourceProject[pProject])){
		token[count++] = resourceProject[pProject];
		pProject++;
		
		//后跟字母或数字 
		while(IsLetter(resourceProject[pProject]) || IsDigit(resourceProject[pProject])){
			token[count++] = resourceProject[pProject];
			pProject++;
		}
		
		token[count] = '\0';
		syn = searchReserve(reserveWords,token);
		
		//如果不是保留字则为标识符 
		if(syn == -1){
			syn = 100;
		}
		return ;
	}
	
	//数字开头 
	else if(IsDigit(resourceProject[pProject])){
		while(IsDigit(resourceProject[pProject])){
			token[count++] = resourceProject[pProject];
			pProject++;
		}
		token[count] = '\0';
		syn = 99;
	}
	
	//运算符 界符开头，查表得结果 
	else if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || 
			ch == ';' || ch == '(' || ch == ')' || ch == '^' ||
          ch == ',' || ch == '\"' || ch == '\'' || ch == '~' ||
		   ch == '#' || ch == '%' || ch == '['  || ch == ']' ||
		    ch == '{' || ch == '}' || ch == '\\'|| ch == '.' ||
			ch == '\?' || ch == ':')
	{
		token[0] = resourceProject[pProject];
		token[1] = '\0';
		for(i = 0; i < 36; i++){
			if(strcmp(token, operatorOrDelimiter[i]) == 0){
				syn = 33 + i;
				break;
			}
		}		 	
		pProject++;
		return ;
	}
	
	// 若为< <= <<
	else if(resourceProject[pProject] == '<'){
		pProject++;
		if(resourceProject[pProject] == '='){
			syn = 38;
		} 
		else if(resourceProject[pProject] == '<'){
			pProject--;
			syn = 58;
		}
		else{
			pProject--;
			syn = 37;
		}
		
		pProject++;
		return;
	} 
	
	// 若为 >,>=,>>
	else  if (resourceProject[pProject] == '>'){
        pProject++;
        if (resourceProject[pProject] == '='){
            syn = 40;
        }
        else if (resourceProject[pProject] == '>'){
            syn = 59;
        }
        else{
            pProject--;
            syn = 39;
        }
        pProject++;
        return;
    }
    
    // 若为= == 
    else if (resourceProject[pProject] == '='){
        pProject++;
        if (resourceProject[pProject] == '='){
            syn = 42;
        }
        else{
            pProject--;
            syn = 41;
        }
        pProject++;
        return;
    }
    
    // 若为! != 
    else if(resourceProject[pProject] == '!'){
        pProject++;
        if(resourceProject[pProject] == '='){
            syn = 43;
        }
        else{
            syn = 68;
            pProject--;
        }
        pProject++;
        return;
    }
    
    //若为& && 
    else  if (resourceProject[pProject] == '&'){
        pProject++;
        if (resourceProject[pProject] == '&'){
            syn = 53;
        }
        else{
            pProject--;
            syn = 52;
        }
        pProject++;
        return;
    }
    
    //若为| || 
    else  if (resourceProject[pProject] == '|'){
        pProject++;
        if (resourceProject[pProject] == '|'){
            syn = 55;
        }
        else{
            pProject--;
            syn = 54;
        }
        pProject++;
        return;
    }
    
    // 若为结束符
    else if (resourceProject[pProject] == '$')
    {
    	//种别码为0
        syn = 0;
    }
    
    //不能被以上词法分析识别，则出错。
    else{
        printf("错误201:非法字符\n", ch);
        exit(0);
    }
}


int main(){
	//源程序输入文件存入该数组 
	char resourceProject[10000];

	//扫描缓存 
	char token[20] = {0}; 

	//$=0,结束 
	int syn = -1 , i; 

	//源程序指针,指向当前扫描位置 
	int pProject = 0; 
	
	FILE *fp,*fp1;
	if((fp = fopen("D:\\test1.txt","r")) == NULL){
		cout<<"错误301:打开文件失败！文件不存在"<<endl;
		exit(0); 
	}
	
	//按字节读取 
	resourceProject[pProject]  = fgetc(fp);
	while(resourceProject[pProject] != '$'){
		pProject++;
		resourceProject[pProject] = fgetc(fp);
	}
	resourceProject[++pProject] = '\0';
	fclose(fp);
	
	//**测试是否读入成功 
	cout<<"源代码:"<<endl;
	cout<<resourceProject<<endl;
	
	//对原文件过滤 
	filterResource(resourceProject,pProject);
	pProject = 0;
	
	//**测试过滤是否成功
	cout<<"过滤后源代码:"<<endl;
	cout<<resourceProject<<endl;
	
	//过滤结果写入文件
	if((fp1 = fopen("D:\\resoult.txt","w+")) == NULL){
		cout<<"错误401:文件写出失败"<<endl;
		exit(0);
	} 
	
	while(syn != 0){
		Scanner(syn,resourceProject,token,pProject);
		
		//扫描到标识符 
		if(syn == 100){
			for(i = 0; i < 100; i++){
				if(strcmp(IdenticalTbl[i],token) == 0){
					//已存在 
					break;
				}
				if(strcmp(IdenticalTbl[i],"") == 0){
					strcpy(IdenticalTbl[i],token);
					break;
				} 
			}
			printf("(标识符  ， %s)\n", token);
			fprintf(fp1,"(标识符  ， %s)\n", token);
		}
		
		//扫描到保留字 
		else if(syn >= 1 && syn <= 32){
			printf("(%s   ,  --)\n", reserveWords[syn - 1]);
            fprintf(fp1, "(%s   ,  --)\n", reserveWords[syn - 1]);
		}
		
		//扫描到常数 
		else if(syn == 99){
			printf("(常数   ,   %s)\n", token);
            fprintf(fp1, "(常数   ,   %s)\n", token);
		}
		
		else if (syn >= 33 && syn <= 68)
        {
            printf("(%s   ,   --)\n", operatorOrDelimiter[syn - 33]);
            fprintf(fp1, "(%s   ,   --)\n", operatorOrDelimiter[syn - 33]);
        }
	}
	
	//插入标识符表 
	for( i = 0; i < 100; i++){
		printf("第%d个标识符：  %s\n", i + 1, IdenticalTbl[i]);
        fprintf(fp1, "第%d个标识符：  %s\n", i + 1, IdenticalTbl[i]);
	}
	
	fclose(fp1);
    return 0;
}	
	
 
