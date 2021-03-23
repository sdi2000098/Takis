/* All 7 MODES have been materialized */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* #define MODE 7 */
#define ERROR -1
void mysound(int dur, int sr, double fm, double fc, double mi, double mv);
void little (int number,int size){                    /* A simple function that fills  bytes with a number in little endian form. Size indicates the number of bytes*/
    int i,x,y,temp;
    i=1;
    do{
        x=number%16;
        number=number/16;
        y=number%16;
        number=number/16;
        putchar(x+y*16);
        i++;
    }while(number != 0);
    for(;i<=size;i++){             /* Fill the remaining bytes with 0 */
        putchar(0);
    }
}
int convert(int size,int change){          /* This functions gets as input the number of bytes for each data type and coverts hexadecimal to decimal (little-endian)*/
                                           /* Int change is used for MODE 2 or 3. It indicates whether the data of the new file must be halved or doubled. 1 stands for YES and 0 for NO. */
    int hex,decimal,i,ch,x,y,temp,stop;
    hex=1;                          /* The term that we are going to multiply the number to convert hexadecimal to decimal */
    decimal=0;                      /* Indicates the number in its decimal form */
    for(i=1;i<=size;i++){           /* For each byte */
        ch=getchar();
        if (((MODE == 2 || MODE ==3 ) && change == 0) || MODE == 6){       /* Copy each byte to the new file without changes */
           putchar(ch);
        }
        decimal+=ch*hex;
        hex*=16*16;
    }
    if(MODE == 2  && change== 1){             /* We have to copy the bytes with is halved value */
        temp=decimal/2;
        i=1;
        little(temp,size);
    }
    if (MODE == 3 && change == 1){		/* Copy the bytes with double value */
        temp=decimal*2;
        little(temp,size);
    }
    return decimal;
}
int read(void){                /* A simple function that reads a byte. If we are in MODE 2,3 or 6 , it also copies it to a new file */
    int ch;
    ch=getchar();
    if ((MODE == 2 || MODE==3 || MODE==6 ) && ch != EOF ){
        putchar(ch);
    }
    return ch;
}
int main(void)
{   if (MODE != 7){
        int ch,sfc,szf,mono,wtf,bps,ba,bpsa,sr,sod,count,hex,copy,temp,i,x,y;
        ch=read();
        if (ch != 'R'){                             /*Checking for "RIFF"*/
            fprintf(stderr,"Error! \"RIFF\" not found\n");
            return ERROR ;                               /* ERROR indicates that something went wrong with the entries of the program */
        }
        ch=read();
        if (ch != 'I' ){
            fprintf(stderr,"Error! \"RIFF\" not found\n");
            return ERROR;
        }
        ch=read();
        if (ch != 'F'){
            fprintf(stderr,"Error! \"RIFF\" not found\n");
            return ERROR;
        }
        ch=read();
        if (ch != 'F'){
            fprintf(stderr,"Error! \"RIFF\" not found\n");
            return ERROR;
        }
        /* Going for SizeOfFile */
        szf=convert(4,0);
        fprintf(stderr,"size of file: %d\n",szf);
        ch=read();              /*Checking for "WAVE"*/
        if (ch != 'W'){
            fprintf(stderr,"Error! \"WAVE\" not found.\n");
            return ERROR;
        }
        ch=read();
        if (ch != 'A'){
            fprintf(stderr,"Error! \"WAVE\" not found.\n");
            return ERROR;
        }
        ch=read();
        if (ch != 'V'){
            fprintf(stderr,"Error! \"WAVE\" not found.\n");
            return ERROR;
        }
        ch=read();
        if (ch != 'E'){
            fprintf(stderr,"Error! \"WAVE\" not found.\n");
            return ERROR;
        }
        ch=read();              /* Going got "fmt "*/
        if (ch != 'f'){
            fprintf(stderr,"Error! \"fmt \" not found.\n");
            return ERROR;
        }
        ch=read();
        if (ch != 'm'){
            fprintf(stderr,"Error! \"fmt \" not found.\n");
            return ERROR;
        }
        ch=read();
        if (ch != 't'){
            fprintf(stderr,"Error! \"fmt \" not found.\n");
            return ERROR;
        }
        ch=read();
        if (ch != ' '){
            fprintf(stderr,"Error! \"fmt \" not found.\n");
            return ERROR;
        }
                                    /* Going for format chunk */
        sfc=convert(4,0);             /* Size of format chunk */
            if (sfc==16){
                fprintf(stderr,"size of format chunk: %d\n",sfc);
            }
            else{
                fprintf(stderr,"size of format chunk: %d\nError! size of chunk should be 16\n",sfc);
                return ERROR;
            }
        /* Going for type of WAVE format */
        wtf=convert(2,0);             /*Wave type format */
        if (wtf == 1){
            fprintf(stderr,"WAVE type format: %d\n",wtf);
        }
        else{
            fprintf(stderr,"WAVE type format: %d\nError! WAVE type format should be 1\n",wtf);
            return ERROR;
        }

        /*Going for the MonoStereo */
        mono=convert(2,0);                 /* Indicates whether sound is mono or stereo */
        fprintf(stderr,"mono/stereo: %d\n",mono);
        if (mono != 1 && mono !=2){
            fprintf(stderr,"Error! mono/stereo should be 1 or 2\n");
            return ERROR;
        }
        /*Going for SampleRate */
        sr=convert(4,1);          /*Indicates sample rate */
        fprintf(stderr,"sample rate: %d\n",sr);
                                  /*Going for BytesPerSec */
        bps=convert(4,1);         /* Indicates bytes per sec */
        fprintf(stderr,"bytes/sec: %d\n",bps);
                                  /* Going for BlockAlign */
        ba=convert(2,0);
        fprintf(stderr,"block aligment: %d\n",ba);
        if (bps != sr*ba){
            fprintf(stderr,"Error! bytes/second should be sample rate x block alignment\n");
            return ERROR;
        }
        /* Going for BitsPerSample */
        bpsa=convert(2,0);
        fprintf(stderr,"bits/sample: %d \n",bpsa);
        if (bpsa != 8 && bpsa != 16){
            fprintf(stderr,"Error! bits/sample should be 8 or 16\n");
            return ERROR;
        }
        if (ba != bpsa/8 * mono){
            fprintf (stderr,"Error! block alignment should be bits per sample / 8 x mono/stereo\n");
            return ERROR;
	}
        /* Going for "data" */
        ch=read();
        if (ch != 'd'){
            fprintf(stderr,"Error! \"data\" not found\n");
            return ERROR;
        }
        ch=read();
        if( ch != 'a'){
            fprintf(stderr,"Error! \"data\" not found\n");
            return ERROR;
        }
        ch=read();
        if( ch != 't'){
            fprintf(stderr,"Error! \"data\" not found\n");
            return ERROR;
        }
        ch=read();
        if( ch != 'a'){
            fprintf(stderr,"Error! \"data\" not found\n");
            return ERROR;
        }
        /* Going for SizeOfData */
        sod=convert(4,0);
        fprintf(stderr,"size of data chunk: %d\n",sod);
        /* Going to check whether data is sufficient or not */
        if (MODE != 4 && MODE != 5 && MODE !=6){
            count=0;                    /* Counts each byte */
            ch=read();
            while (ch != EOF){          /*Reading the remaining data of the file*/
                count++;
                ch=read();
            }
            if (sod > count){         /* If the bytes that were read are insufficient */
                fprintf(stderr,"Error! insufficient data\n");
                return ERROR;
            }
            /* Going to check whether they bytes that were read match with the size of file */
            if ((count) != (szf - 36)){
            /* Count is the number of bytes used for data chunk */
            /* szf -36 is the size of file minus the number of bytes used for the information of the file */
                fprintf(stderr,"Error! bad file size\n");
                return ERROR;
            }
        }
        else if (MODE == 4 || MODE == 5){   /* We will create a new acceptable wav file */
                putchar('R');               /* Inserting RIFF to the new file */
                putchar('I');
                putchar('F');
                putchar('F');
                /* Going for SizeOfFile*/
                if (mono == 2){             /*In this case the new SizeOfFile will be the original size minus half of the size of data */
                    temp=szf-sod/2;         /* The new size of file */
                }
                else{
                    temp=szf;               /*In this case the new SizeOfFile will be the same with original */
                }
                little(temp,4);             /* Insert the SizeOfFile to the new file using 4 bytes */
                /* Going for WAVE */
                putchar('W');
                putchar('A');
                putchar('V');
                putchar('E');
                /*Going for fmt */
                putchar('f');
                putchar('m');
                putchar('t');
                putchar(' ');
                /*Going for szf, in each case it is equal to 16*/
                little(16,4);
                /*Going for wtf, in each case it is equal to 1 */
                little(1,2);
                /*Going for mono/stereo, in each case it is equal to 1*/
                little(1,2);
                /* Going for SampleRate, in each case it will be the same with original */
                temp=sr;
                little(temp,4);
                /* Going for bps. */
                if (mono == 2){     /* Bps are going to be halved */
                    temp=bps/2;
                }
                else{               /* Bps are going to be the same with original */
                    temp=bps;
                }
                little(temp,4);
                /* Going for BlockAlign */
                if (mono == 2){     /* In this case the BlockAlign will be halved */
                   temp=ba/2;
                }
                else{               /* In this case BlockAlign remains the same */
                    temp=ba;
                }
                little(temp,2);
                /* Going for BitsPerSample, in each case they remain the same */
                temp=bpsa;
                little(temp,2);
                /* Going for data */
                putchar('d');
                putchar('a');
                putchar('t');
                putchar('a');
                /* Going for SizeOfData */
                if (mono == 2){     /* Size of data will be halved */
                   temp=sod/2;
                }
                else{               /* Size of data remains the same */
                    temp=sod;
                }
                little(temp,4);
                /* Going to read the data */
                count=0;
                if (mono == 1){         /* In this case we just copy data as they are */
                    do{
                        ch=getchar();
                        if (ch!= EOF){
                            putchar(ch);
                            count++;
                        }
                    }while (ch !=EOF );
                    if (sod > count){         /* If the bytes that were read are insufficient */
                        fprintf(stderr,"Error! insufficient data\n");
                        return ERROR;
                    }
                    /* Going to check whether they bytes that were read match with the size of file */
                    if ((count) != (szf - 36)){
                        /* Count is the number of bytes used for data chunk */
                        /* szf -36 is the size of file minus the number of bytes used for the data of the file */
                        fprintf(stderr,"Error! bad file size\n");
                        return ERROR;
                    }
                }
                else{                   /* In this case depending the mode we will copy only the right or the left channel */
                    if (MODE == 4){     /* The first sample corresponds to the left channels, in MODE 4 the first sample must be copied */
                        copy=1;         /* Tis variable determines whether the bytes is going to be copied or not. 1 stand for YES and -1 for NO. */
                    }
                    else{               /* In mode 5 we will copy the right channel, so we have to skip the first sample */
                       copy=-1;
                    }
                    do{                 /* Now we are going to copy the data */
                        for(i=1;i<=ba/2;i++){       /* The number of bytes that are necessary to express the sound in each moment for one channel */
                            ch=getchar();
                            if (ch!= EOF && copy == 1){
                               putchar(ch);
                               count++;
                            }
                        }
                        copy*=-1;      /* Change variable's value to it's opposite, so to skip bytes that must not be copied */
                    }while (ch!=EOF && count*2<sod);     /* Read as many bytes as the size of file */
                    if (MODE == 4 && ch!= EOF){			/* Read the remaining bytes of the right channel */
                        for (i=1;i<=ba/2;i++){
                            getchar();
                        }
                    }
                    if (sod > count*2){         /* If the bytes that were read are insufficient */
                        fprintf(stderr,"Error! insufficient data\n");
                        return ERROR;
                    }
                    while (ch!=EOF){        /*In case there are some more bytes that must be copied */
                        ch=getchar();
                        if(ch!=EOF){
                            putchar(ch);
                            count++;
                        }
                    }
                    if (count != (szf-36 - sod/2)){
                        fprintf(stderr,"Error! bad file size\n");
                        return ERROR;
                    }
                }
            }
            else if (MODE==6){          /* We will copy each byte to the new file divided by 8 */
                count=0;
                do{
                    hex=1;
                    temp=0;
                    for (i=1;i<=bpsa/8;i++){        /* We will read each sample for every moment depending on the number of bytes used to store each sample */
                       ch=getchar();
                       if ( ch == EOF ){
                        break;
                       }
                       temp+=ch*hex;
                       hex*=16*16;
                       count++;
                    }
                    if (ch != EOF){
			if (temp <= 32767 ){	/* The number is inside the limits */
                       	     temp=temp/8;
			     little(temp,bpsa/8);     /* Covert to little endian. Use as many bytes needed for each sample. */
			}
			else{			/* The number should be negative. In order to represent in hexadecimal two's complement of two byes, we can store the number 2^16-(the number we want to store). Its representation will be the same */
				temp=(signed short ) temp / 8 +65536;
				little(temp,bpsa/8);
			}
		   }
                }while (count<sod && ch!=EOF);

                if (sod>count){
                    fprintf(stderr,"Error! insufficient data\n");
                    return ERROR;
                }
                /* Read the remaining bytes, if they exist, and copy them */
                while(ch != EOF){
                   ch= getchar();
                    if (ch != EOF){
                        putchar(ch);
                        count++;
                    }
                }
                if ((count) != (szf - 36)){
                /* Count is the number of bytes used for data chunk */
                /* szf -36 is the size of file minus the number of bytes used for the data of the file */
                    fprintf(stderr,"Error! bad file size\n");
                    return ERROR;
                }
            }
    }
    else if (MODE == 7){
        mysound(3, 44100, 2.0, 1500.0, 100.0,30000.0);
    }
    return 0;
}


void mysound(int dur, int sr, double fm, double fc, double mi, double mv){
    int ba,bpsa=16,mono=1,bps,sod,sfc=16,wtf=1,i,temp,sof,x,y;
    double t,sound;
    /*Inserting 'RIFF' to the new file */
    putchar('R');
    putchar('I');
    putchar('F');
    putchar('F');
    /* First we need to calculate Bytes Per Sec, Block Align and Size of Data */
    /*BlockAlign = BitsPerSample/8 × MonoStereo.*/
    ba=bpsa/8*mono;
    /*BytesPerSec = SampleRate × BlockAlign*/
    bps=sr*ba;
    /* SizeOfData = BytesPerSec*Seconds */
    sod=bps*dur;
    /* SizeOfFile = SizeOfData + 36 */
    sof=sod+36;
    fprintf(stderr,"size of file: %d\n",sof);
    /* Going to insert SizeOfFile to the news file */
    little(sof,4);
    /* Going for 'WAVE' */
    putchar('W');
    putchar('A');
    putchar('V');
    putchar('E');
    /* Going for 'fmt ' */
    putchar('f');
    putchar('m');
    putchar('t');
    putchar(' ');
    /* Going for SizeOfFormatChunk */
    fprintf(stderr,"size of format chunk: %d\n",sfc);
    little(sfc,4);
    /* Going for WaveTypeFormat */
    fprintf(stderr,"WAVE type format: %d\n",wtf);
    little(wtf,2);
    /*Going for mono-stereo */
    fprintf(stderr,"mono/stereo: %d\n",mono);
    little(mono,2);
    /* Going for SampleRate */
    fprintf(stderr,"sample rate: %d\n",sr);
    little(sr,4);
    /*Going for bytes/sec */
    fprintf(stderr,"bytes/sec: %d\n",bps);
    little(bps,4);
    /*Going for BlockAlignment */
    fprintf(stderr,"block alignment: %d\n",ba);
    little(ba,2);
    /* Going for bits/sample */
    little(bpsa,2);
    fprintf(stderr,"bits/sample: %d\n",bpsa);
    /*Going for 'data' */
    putchar('d');
    putchar('a');
    putchar('t');
    putchar('a');
    /* Going for SizeOfDataChunk */
    fprintf(stderr,"size of data chunk: %d\n",sod);
    little(sod,4);
    /* Now we have to produce the the sound data */
    for(t=0.0;t<=(double)dur;t+=1.0/(double)sr){
        /* For each second, SampleRate determines the number of sound data */
        sound=mv*sin(2*M_PI*fc*t-mi*sin(2*M_PI*fm*t));
        temp=(int)sound;        /* Storing sound data to a new int variable */
        /* BlockAlign determines the number of bytes that are used to store each sound data */
        if ( temp < 0 ){          /* Convert to two's complement */
		temp=65536+temp;
        }
        for (i=1;i<=ba;i++){
            /* Covert to little endian */
            x=temp%16;
            temp=temp/16;
            y=temp%16;
            temp=temp/16;
            putchar(x+y*16);
        }
    }
}
