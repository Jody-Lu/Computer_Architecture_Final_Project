#include<iostream.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<fstream.h>

char address[80];
ifstream file;
long int line;
 char ch,prediction;
 long int misses = 0;
 long int traces = 0;

 int shifter,i,rows;
 int indexno,M;
 int *predictor;
 void bimodal(int);
 float missrate;
 char filename[10];
 fstream file2;

int* dectobin(int,int);
int bintodec(int *,int);
void updateBHR(int,int);
 int *BHR;
 int shifter2;
 int tagPC;
 int decBHR;
 int N;
 int *PC, *PC1, *PC2, *finalindex, *XOR;
 int findex,in1;
 int PC_n;
 int* PC_n_bin;
 void gshare(int,int);


 int main(int argc, char *argv[])
{ using namespace std;
 
  if(!strcmp(argv[1],"bimodal"))
  { M = atoi(argv[2]);
    file.open(argv[3]);
    strcpy(filename,argv[3]);
    bimodal(M);
  }


  if(!strcmp(argv[1],"gshare"))
 { M = atoi(argv[2]);
   N = atoi(argv[3]);
  file2.open(argv[4]);
   strcpy(filename,argv[4]);
  gshare(M,N);
 }
    

 return 0;
 }


 


void bimodal(int m)
 {  

 shifter = pow(2,m)-1;
 rows = shifter + 1;
 


/*Create a predictor table of counters*/
 predictor = new int[rows];
 
 if(predictor == NULL)
 cout<<"Could not allocate memory";

 
/*---Initialize predictor table to all 2's ----*/
 for(i=0;i<rows;i++)
 predictor[i]=2;


 
/*----Read Addresses----*/
 file>>address;
 
  
   while(!file.eof())
{
  line=strtol(address,'\0', 16);
  indexno = (line >> 2) & (shifter);
  file>>ch;
  traces++;  
  //cout<<"\n"<<address<<"   "<<ch;
  
  /*----predict branch's outcome---*/
  switch(predictor[indexno])
  { 
    case 0: prediction = 'n';
            //cout<<"\n\t"<<"BP:  "<<indexno<<"  "<<predictor[indexno];
            if(ch == 'n')
            { //cout<<"\nPrediction correct";
               predictor[indexno] = 0;
            }
            
            else if(ch=='t')
            { //cout<<"\nPrediction incorrect";
              predictor[indexno] = 1;
              misses++;
            }
            //cout<<"\n\t"<<"BU:  :"<<indexno<<"  "<<predictor[indexno];
            break;
  
    
    case 1: prediction = 'n';
            //cout<<"\n\t"<<"BP:  "<<indexno<<"  "<<predictor[indexno];
            if(ch == 'n')
            { //cout<<"\nPrediction correct";
              predictor[indexno] = 0;
            }
            
            else if(ch=='t')
            { //cout<<"\nPrediction incorrect";
              predictor[indexno] = 2;
              misses++;
            }

            //cout<<"\n\t"<<"BU:  "<<indexno<<"  "<<predictor[indexno];
            break;


    case 2: prediction = 't';
            //cout<<"\n\t"<<"BP:  "<<indexno<<"  "<<predictor[indexno];

            if(ch == 'n')
            { //cout<<"\nPrediction incorrect";
              predictor[indexno] = 1;
              misses++;
            }
            
            else if(ch=='t')
            { //cout<<"\nPrediction correct";
              predictor[indexno] = 3;
            }

            //cout<<"\n\t"<<"BU:  "<<indexno<<"  "<<predictor[indexno];
            break;

    
     case 3: prediction = 't';
             //cout<<"\n\t"<<"BP:  "<<indexno<<"  "<<predictor[indexno];

            if(ch == 'n')
            { //cout<<"\nPrediction incorrect";
              predictor[indexno] = 2;
              misses++;
            }
            
            else if(ch=='t')
            { //cout<<"\nPrediction correct";
              predictor[indexno] = 3;
            }

            //cout<<"\n\t"<<"BU:  "<<indexno<<"  "<<predictor[indexno];
            break;
  
  
   default : cout<<"\n\nInvalid counter value";
 }     //switch


 file>>address;
}     //while eof 


missrate = ((float)misses*100/(float)traces);


/*----print-----*/

cout<<"COMMAND\n";
cout<<"./sim bimodal "<<m<<" "<<filename<<endl;
cout<<"OUTPUT\n";
cout<<" number of predictions:    "<<traces;
cout<<"\n number of mispredictions: "<<misses;
printf("\n misprediction rate:       %2.2f%",missrate);
cout<<"%";
cout<<"\nFINAL BIMODAL CONTENTS\n";



for(i=0;i<rows;i++)
cout<<i<<"	"<<predictor[i]<<endl;


delete[] predictor; 
 
}



           
 
void gshare(int m,int n)
{


 /*----Allocate BHR---*/
 BHR = new int[n];

 if(BHR == NULL)
 cout<<"Could not allocate memory";


 /*----Allocate PC---*/
 PC = new int[m];

 if(PC == NULL)
 cout<<"Could not allocate memory";


 /*----Allocate PC2---*/
 PC2 = new int[m-n];

 if(PC2 == NULL)
 cout<<"Could not allocate memory";

 /*----Allocate XOR---*/
 XOR = new int[n];

 if(XOR == NULL)
 cout<<"Could not allocate memory";

 /*----Allocate finalindex---*/
 finalindex = new int[m];

 if(finalindex == NULL)
 cout<<"Could not allocate memory";


 /*---Initialize to all zeros---*/
 for(i=0;i<n;i++)
 BHR[i]=0;

 /*---Allocate PC1---*/
 PC1 = new int[n];

 if(PC1 == NULL)
 cout<<"Could not allocate memory";

  
 shifter = pow(2,m)-1;
 int rows = shifter + 1;

 /*---Allocate predictor---*/
 predictor = new int[rows];

 if(predictor == NULL)
 cout<<"Could not allocate memory";


 /*---Initialize predictor---*/
 for(i=0;i<rows;i++)
  predictor[i]=2;


 file2>>address;
 
 while(!file2.eof())
 {
  line=strtol(address,'\0', 16);
 
  in1 = (line>>2) & (shifter);
  PC = dectobin(in1,m);
 
  PC2 = PC + n;
 
 
 for(i=0;i<n;i++)
 PC1[i]=PC[i];
 
 
 for(i=0;i<n;i++)
 {XOR[i] = BHR[i]^PC1[i];} 
 
 
 /*---make final index---*/
 
 for(i=0;i<n;i++)
 { finalindex[i] = XOR[i];}
 
 for(i=0;i<(m-n);i++)
 { finalindex[n+i] = PC2[i];}

 
 findex = bintodec(finalindex,m);
 //cout<<"\n"<<findex;    

 file2>>ch;
 //cout<<"\n"<<address<<"   "<<ch;
  traces++;

  switch(predictor[findex])
 {
  case 0: prediction = 'n';
          //cout<<"\n\t"<<"GP:  "<<findex<<"  "<<predictor[findex];
          if(ch=='n')
         { //cout<<"\nPrediction correct";
           predictor[findex] = 0;
          updateBHR(n,0);
          }

         else if(ch=='t')
         { //cout<<"\nPrediction incorrect";
           predictor[findex] = 1;
           misses++;
           updateBHR(n,1);
         }

         //cout<<"\n\t"<<"GU:  "<<findex<<"  "<<predictor[findex];
         break;

 
   case 1: prediction = 'n';
           //cout<<"\n\t"<<"GP:  "<<findex<<"  "<<predictor[findex];
          if(ch=='n')
         { //cout<<"\nPrediction correct";
           predictor[findex] = 0;
           updateBHR(n,0);
         }

         else if(ch=='t')
         { //cout<<"\nPrediction incorrect";
           predictor[findex] = 2;
           misses++;
           updateBHR(n,1);
         }
         //cout<<"\n\t"<<"GU:  "<<findex<<"  "<<predictor[findex];
         
          break;

 
   case 2: prediction = 't';
           //cout<<"\n\t"<<"GP:  "<<findex<<"  "<<predictor[findex];
           if(ch=='n')
         { //cout<<"\nPrediction incorrect";
           predictor[findex] = 1;
           misses++;
           updateBHR(n,0);
         }

         else if(ch=='t')
         { //cout<<"\nPrediction correct";
           predictor[findex] = 3;
           updateBHR(n,1);
         }
         //cout<<"\n\t"<<"GU:  "<<findex<<"  "<<predictor[findex];
         
         break;

 
    case 3: prediction = 't';
           // cout<<"\n\t"<<"GP:  "<<findex<<"  "<<predictor[findex];
          if(ch=='n')
         { //cout<<"\nPrediction incorrect";
           predictor[findex] = 2;
           misses++;
           updateBHR(n,0);
         }

         else if(ch=='t')
         { //cout<<"\nPrediction correct";
           predictor[findex] = 3;
           updateBHR(n,1);
          }
         //cout<<"\n\t"<<"GU:  "<<findex<<"  "<<predictor[findex];
         
         break;

   default : cout<<"\nInvalid counter value";
 
  } //switch



 file2>>address;
 }  //while


missrate=((float)misses*100/(float)traces);

 /*---print---*/

cout<<"COMMAND\n";
cout<<"./sim gshare "<<m<<" "<<n<<" "<<filename<<endl;
cout<<"OUTPUT\n";
cout<<" number of predictions:    "<<traces;
cout<<"\n number of mispredictions: "<<misses;
printf("\n misprediction rate:       %2.2f",missrate);
cout<<"%";
cout<<"\nFINAL GSHARE CONTENTS\n";



for(i=0;i<rows;i++)
cout<<i<<"	"<<predictor[i]<<endl;




}




 
int bintodec(int *b, int size)
{ 
 int j=0;
 int sum=0;
 
 for(j=0;j<size;j++) 
 { 
   sum=sum + b[j]*pow(2,(size-1-j));
   
 }
 
 
 return sum;
}



int* dectobin(int d,int size)
{ int *bin;
  int j;

 /*----Allocate bin---*/
 bin = new int[size];

 if(bin == NULL)
 cout<<"Could not allocate memory";

 
 for(j=0;j<size;j++)
 { bin[size-1-j]=d%2;
   d/=2;
 }

 return bin;
} 
 

void updateBHR(int size,int outcome)
{
 int j;
 for(j=size-1;j>0;j--)
 { BHR[j] = BHR[j-1];}

 BHR[0]=outcome;
}
