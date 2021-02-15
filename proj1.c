
#include <stdio.h>
#include <string.h>

typedef struct instructions{				//Struct to hold registers for instructions
	char label[10];
	char instruct[10];
	char reg1[4];
	char reg2[4];
	char reg3[4];	
	int  addr;
	char type;
}instructions;

int main()
{
int end=0;
int bool =0;
int add=0;
int check=0;
struct instructions mips[99];				//array to hold maximum of 100 instructions

char *p;
int space;						//store values tied to .space
char *ptr;

char store[80];
int count=0;
const char whitespace[7]= " ,:\t\n()";			//store delimiters
char *token;						//stores input file lines

while(fgets(store,80,stdin)){
mips[count].addr=add;					//sets value of instructions address

if(strstr(store,":")){
token=strtok(store,whitespace);
strcpy(mips[count].label,token);
token=strtok(NULL,whitespace);
strcpy(mips[count].instruct,token);			//set stdin to token



}

else{							//tokenate and copy lines from stdin into according registers
	token=strtok(store,whitespace);
        strcpy(mips[count].instruct,token);

}
	token=strtok(NULL,whitespace);
	

	if(token!=NULL)
	strcpy(mips[count].reg1,token);
        token=strtok(NULL,whitespace);
        if(token!=NULL)
        strcpy(mips[count].reg2,token);
        token=strtok(NULL,whitespace);
        if(token!=NULL)
        strcpy(mips[count].reg3,token);


if(strcmp(mips[count].instruct,"la")==0){		//if instruction is la, divide into lui and ori

char  reggi[4];
char  label [4];
mips[count].type='I';
strcpy(reggi,mips[count].reg1);
strcpy(label,mips[count].reg2);

check =0;
strcpy(mips[count].instruct,"lui");			//distribute registers
strcpy(mips[count+1].instruct,"ori");
strcpy(mips[count].reg1,"$1");
strcpy(mips[count].reg2,label);
strcpy(mips[count+1].reg1,reggi);
strcpy(mips[count+1].reg2,"$1");
strcpy(mips[count+1].reg3,label);

add+=4;
mips[count+1].addr=add;					//increment address

count++;						//increment count
check++;						//increase to fixe address
}

if(count>=1){						//set address
mips[count].addr=add;
add+=4;


}

if(check>0)						//adjust adresses for directives errors
add-+8;
check=0;

if(strcmp(mips[count].instruct,".space")==0){		//adjust allocated bits for .space
space=atoi(mips[count].reg1);
//add+=space;
mips[count].type='D';
}

if(strstr(mips[count].instruct,".data")!=NULL)		//ignore data directive
add-=4;

				//SET TYPES												

 if(strcmp(mips[count].instruct,"add")==0||strcmp(mips[count].instruct,"nor")==0||strcmp(mips[count].instruct,"sll")==0)
mips[count].type='R';

else if(strcmp(mips[count].instruct,"j")==0)
mips[count].type='J';

else if(strcmp(mips[count].instruct,"add")!=0&&strcmp(mips[count].instruct,"nor")!=0&&strcmp(mips[count].instruct,"sll")!=0&&strcmp(mips[count].label,"j")!=0&&strcmp(mips[count].instruct,".text")!=0&&strcmp(mips[count].instruct,".data")!=0)
mips[count].type='I';

ptr =strchr(mips[count].instruct,'.');

if(ptr!=NULL)
mips[count].type='D';





count++;



}//while



int i=0;
for(i=1;i<count;i++){								//calculate the instruction values for each line and print alongside address


if(mips[i].type!='D'){
printf("\n0x%08X:",mips[i].addr);
		
		if(mips[i].type=='J'){				//print J types
		int jump = check_value(mips[i].instruct);
                int imm=0;
		int j;

                for(j=0;j<count;j++){
                        if(strstr(mips[i].reg1,mips[j].label)!=NULL)
                         imm=(mips[j].addr)/4;
                        }//for
        		

			printf(" 0x%08X",jump<<26|imm);		

}// Jump


                if(mips[i].type=='R'){				//print r types
		int j=0;

		if(strstr(mips[i].instruct,"sll")!=NULL){
		int rt =check_value(mips[i].reg2);
		int rd = check_value(mips[i].reg1);
		int sa= atoi(mips[i].reg3);
		 printf(" 0x%08X",rt<<16|rd<<11|sa<<6);	
		}//if	

		else{
                int inst = check_value(mips[i].instruct);
                int rs = check_value(mips[i].reg2);
                int rt = check_value(mips[i].reg3);
                int rd = check_value(mips[i].reg1);
		
		
                printf(" 0x%08X",0<<26|rs<<21 | rt<<16 | rd<<11 |0<<5| inst);
	}



		

                        }// R if


                if(mips[i].type=='I'){		//print i types with lui and ori distinction
			int j;

if(strstr(mips[i].instruct,"lui")!=NULL){
                
                int inst=check_value(mips[i].instruct);
                int rt = check_value(mips[i].reg1);
                int imm;
                        for(j=0;j<count;j++){
                        if(strstr(mips[j].label,mips[i].reg2)!=NULL)
                         imm=mips[j].addr>>16;

                        }//for
printf(" 0x%08X",inst<<26|0<<5|rt<<16|imm<<16);
                }//lui



else if(strcmp(mips[i].instruct,"ori")==0){
		
                int inst=check_value("ori");
                int rt =check_value(mips[i].reg1);
                int rs = check_value(mips[i].reg2);
                int imm=0;
		
		unsigned r=0;
			
                for(j=0;j<count;j++){
			
                        if(strstr(mips[i].reg3,mips[j].label)!=NULL){
                         	
	
			imm=mips[j].addr;
			}
                        }//for

                printf(" 0x%08X",inst<<26|rs<<21|rt<<16|imm&0xFFFF);


                }//ori
else if(strstr(mips[i].instruct,"bne")!=NULL){
		
		int j;
		int inst=check_value(mips[i].instruct);
		int rs = check_value(mips[i].reg1);
		int rt = check_value(mips[i].reg2);
		int imm;
		for(j=0;j<count;j++){
                        if(strstr(mips[i].reg3,mips[j].label)!=NULL)
                         imm=mips[j].addr;

                        }//for
	
		printf(" 0x%08X",inst<<26|rs<<21|rt<<16|imm);

}

else if(strstr(mips[i].instruct,"add")==NULL){
                int inst = check_value(mips[i].instruct);
                int base = check_value(mips[i].reg3);
                int offset = atoi(mips[i].reg2);
                int rt = check_value(mips[i].reg1);

                printf(" 0x%08X",inst<<26|base<<21|rt<<16|offset);
		}//else


else {
		int inst=check_value(mips[i].instruct);
		int rd=check_value(mips[i].reg1);
		int rs=check_value(mips[i].reg2);
		int imm=atoi(mips[i].reg3);
		printf(" 0x%08X",inst<<26|rs<<21|rd<<16|imm);
		

	}//else

                }//I if

		

}//if



//test(mips[i]);





}//for loop
printf("\n");
return 0;
}

int check_value(char * target)	//retrieve instruction value for all registers and instructions
{
       



	if(strcmp(target,"add")==0)return 32 ;
	if(strcmp(target,"addi")==0)return 8 ;
	if(strstr(target,"nor")!=NULL)return 39 ;
	if(strstr(target,"ori")!=NULL)return 13 ;   
	if(strstr(target,"sll")!=NULL)return 0 ;
	if(strstr(target,"lui")!=NULL)return 15 ;
	if(strstr(target,"sw")!=NULL)return 43 ;         
	if(strstr(target,"lw")!=NULL)return  35;   
	if(strstr(target,"bne")!=NULL)return 5 ;   
	if(strstr(target,"j")!=NULL)return 2 ;   
	if(strstr(target,"$t0")!=NULL) return 8;
	if(strstr(target,"$t1")!=NULL)return 9 ;
	if(strstr(target,"$t2")!=NULL)return 10 ;
	if(strstr(target,"$t3")!=NULL)return 11 ;
	if(strstr(target,"$t4")!=NULL)return 12 ;
	if(strstr(target,"$t5")!=NULL)return 13 ;
	if(strstr(target,"$t6")!=NULL)return 14 ; 
	if(strstr(target,"$t7")!=NULL)return 15 ;
	if(strstr(target,"$s0")!=NULL)return 16 ;
        if(strstr(target,"$s1")!=NULL)return 17 ;
        if(strstr(target,"$s2")!=NULL)return 18 ;
        if(strstr(target,"$s3")!=NULL)return 19 ;
        if(strstr(target,"$s4")!=NULL)return 20 ;  
        if(strstr(target,"$s5")!=NULL)return 21 ;
        if(strstr(target,"$s6")!=NULL)return 22 ;
        if(strstr(target,"$s7")!=NULL)return 23 ;
        if(strstr(target,"$0")!=NULL)return 0 ;
        if(strstr(target,"$1")!=NULL)return 1 ;
}
