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

//��ʶ�� 
static char IdenticalTbl[1000][50] = {""};

//���ұ����� �����ҳɹ� ����ʶ���� ���򷵻�-1 ��Ϊ��ʶ�� 
int searchReserve(char reserveWords[][20], char s[]){
	int i;
	for(i = 0; i < 32; i++){
		if(strcmp(reserveWords[i],s) == 0){
			return i + 1;
		}
	}
	return -1; 
} 

//�Ƿ�����ĸ
bool IsLetter(char letter){
	if(letter >= 'a' && letter <= 'z' || letter >= 'A' && letter <= 'Z'
			|| letter == '_'){
		return true;
	}
	else{
		return false;
	}
} 

//�Ƿ�Ϊ���� 
bool IsDigit(char digit){
	if(digit >= '0' && digit <= '9'){
		return true;
	}
	else{
		return false;
	}
} 

//ȥ�������ַ���ע��
void filterResource(char r[],int pProject){
	char tempString[10000];
	int count = 0;
	int i;
	for(i = 0; i <= pProject; i++){
		//�Ƿ�Ϊ����ע�� 
		if(r[i] == '/' && r[i+1] == '/'){
			while(r[i] != '\n'){
				i++;
			}
		}
		
		//�Ƿ�Ϊ����ע��
		if(r[i] == '/' && r[i+1] == '*'){
			i += 2;
			while(r[i] != '*' || r[i+1] != '/'){
				i++;
				if(r[i] == '$'){
					cout<<"����101:û���ҵ�ע�ͽ�����\n"<<endl;
					exit(0);
				}
			}
			//���� */ 
			i += 2;
		}
		
		//�Ƿ�Ϊ�����ַ� 
		if(r[i] != '\n' && r[i] != '\t' && r[i] != '\v' && r[i] != '\r'){
				tempString[count++] = r[i];
			} 
		
		}
		
		tempString[count] = '\0';
		strcpy(r,tempString);
	
}

//ɨ���� ����DFA��� 
void Scanner(int &syn, char resourceProject[],char token[],int &pProject){
	//count ����token[]��ָʾ��,�ռ���Ч�ַ� 
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
	
	//��ĸ��ͷ 
	if(IsLetter(resourceProject[pProject])){
		token[count++] = resourceProject[pProject];
		pProject++;
		
		//�����ĸ������ 
		while(IsLetter(resourceProject[pProject]) || IsDigit(resourceProject[pProject])){
			token[count++] = resourceProject[pProject];
			pProject++;
		}
		
		token[count] = '\0';
		syn = searchReserve(reserveWords,token);
		
		//������Ǳ�������Ϊ��ʶ�� 
		if(syn == -1){
			syn = 100;
		}
		return ;
	}
	
	//���ֿ�ͷ 
	else if(IsDigit(resourceProject[pProject])){
		while(IsDigit(resourceProject[pProject])){
			token[count++] = resourceProject[pProject];
			pProject++;
		}
		token[count] = '\0';
		syn = 99;
	}
	
	//����� �����ͷ�����ý�� 
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
	
	// ��Ϊ< <= <<
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
	
	// ��Ϊ >,>=,>>
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
    
    // ��Ϊ= == 
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
    
    // ��Ϊ! != 
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
    
    //��Ϊ& && 
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
    
    //��Ϊ| || 
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
    
    // ��Ϊ������
    else if (resourceProject[pProject] == '$')
    {
    	//�ֱ���Ϊ0
        syn = 0;
    }
    
    //���ܱ����ϴʷ�����ʶ�������
    else{
        printf("����201:�Ƿ��ַ�\n", ch);
        exit(0);
    }
}


int main(){
	//Դ���������ļ���������� 
	char resourceProject[10000];

	//ɨ�軺�� 
	char token[20] = {0}; 

	//$=0,���� 
	int syn = -1 , i; 

	//Դ����ָ��,ָ��ǰɨ��λ�� 
	int pProject = 0; 
	
	FILE *fp,*fp1;
	if((fp = fopen("D:\\test1.txt","r")) == NULL){
		cout<<"����301:���ļ�ʧ�ܣ��ļ�������"<<endl;
		exit(0); 
	}
	
	//���ֽڶ�ȡ 
	resourceProject[pProject]  = fgetc(fp);
	while(resourceProject[pProject] != '$'){
		pProject++;
		resourceProject[pProject] = fgetc(fp);
	}
	resourceProject[++pProject] = '\0';
	fclose(fp);
	
	//**�����Ƿ����ɹ� 
	cout<<"Դ����:"<<endl;
	cout<<resourceProject<<endl;
	
	//��ԭ�ļ����� 
	filterResource(resourceProject,pProject);
	pProject = 0;
	
	//**���Թ����Ƿ�ɹ�
	cout<<"���˺�Դ����:"<<endl;
	cout<<resourceProject<<endl;
	
	//���˽��д���ļ�
	if((fp1 = fopen("D:\\resoult.txt","w+")) == NULL){
		cout<<"����401:�ļ�д��ʧ��"<<endl;
		exit(0);
	} 
	
	while(syn != 0){
		Scanner(syn,resourceProject,token,pProject);
		
		//ɨ�赽��ʶ�� 
		if(syn == 100){
			for(i = 0; i < 100; i++){
				if(strcmp(IdenticalTbl[i],token) == 0){
					//�Ѵ��� 
					break;
				}
				if(strcmp(IdenticalTbl[i],"") == 0){
					strcpy(IdenticalTbl[i],token);
					break;
				} 
			}
			printf("(��ʶ��  �� %s)\n", token);
			fprintf(fp1,"(��ʶ��  �� %s)\n", token);
		}
		
		//ɨ�赽������ 
		else if(syn >= 1 && syn <= 32){
			printf("(%s   ,  --)\n", reserveWords[syn - 1]);
            fprintf(fp1, "(%s   ,  --)\n", reserveWords[syn - 1]);
		}
		
		//ɨ�赽���� 
		else if(syn == 99){
			printf("(����   ,   %s)\n", token);
            fprintf(fp1, "(����   ,   %s)\n", token);
		}
		
		else if (syn >= 33 && syn <= 68)
        {
            printf("(%s   ,   --)\n", operatorOrDelimiter[syn - 33]);
            fprintf(fp1, "(%s   ,   --)\n", operatorOrDelimiter[syn - 33]);
        }
	}
	
	//�����ʶ���� 
	for( i = 0; i < 100; i++){
		printf("��%d����ʶ����  %s\n", i + 1, IdenticalTbl[i]);
        fprintf(fp1, "��%d����ʶ����  %s\n", i + 1, IdenticalTbl[i]);
	}
	
	fclose(fp1);
    return 0;
}	
	
 
