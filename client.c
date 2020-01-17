/*
 * client.c
 * 
 * Date: 12/11/2019
 * Author: Dingbang Chen and Tien Li Shen
 *
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"

int main(int argc, char **argv) 
{
    int clientfd;
    char *num1, *num2;
    char *host, *port;
    char buf[MAXLINE];

    if (argc != 3) {
        fprintf(stderr, "usage: %s <num1> <num2>\n", argv[0]);
        exit(0);
    }
    //prepare the message
    num1 = argv[1];
    num2 = argv[2];
    char statement1[] = "POST /RPC2 HTTP/1.1\r\nHost: localhost:8080\r\nAccept-Encoding: gzip\r\nUser-Agent: xmlrpclib.py/1.0.1 (by www.pythonware.com)\r\nContent-Type: text/xml\r\nContent-Length: 218\r\n\r\n<?xml version='1.0'?>\n<methodCall>\n<methodName>sample.addmultiply</methodName>\n<params>\n<param>\n<value><double>";
    char statement2[] = "</double></value>\n</param>\n<param>\n<value><double>";
    char s3[] = "</double></value>\n</param>\n</params>\n</methodCall>\n";
    char dataFind[] = "<value><array><data>\n";
    char dataEnd[] = "</data></array></value>\n";
    char senddata[1024];


    // xml state
    strcpy(buf,"POST /RPC2 HTTP/1.1");
    strcat(buf,"\nUser-Agent: client/1.0");
    strcat(buf,"\nHost: localhost:8080");
    strcat(buf,"\nContent-Type: text/xml");
    strcat(buf,"\nContent-Length: 192");
    strcat(buf,"\n<methodName>sample.addmultiply</methodName>");
    strcat(buf,"\n<params>\n<param>");
    strcat(buf,"\n<value><double>");
    char *charptr = statement1;


    //check of different method getting from client
    int senddataCount = 0;
    while(*charptr != '\0'){
        senddata[senddataCount] = *charptr;
        charptr++;
        senddataCount++;
    }
    charptr = num1;
    int checkDec = 0;
    while(*charptr != '\0'){
        if(*charptr == '.'){checkDec = 1;}
        senddata[senddataCount] = *charptr;
        charptr++;
        senddataCount++;
    }
    if(checkDec == 0){
        senddata[senddataCount] = '.';
        senddata[senddataCount+1] = '0';
        senddataCount = senddataCount + 2;
    }
    checkDec = 0;
    charptr = statement2;
    while(*charptr != '\0'){
        senddata[senddataCount] = *charptr;
        charptr++;
        senddataCount++;
    }
    charptr = num2;
    while(*charptr != '\0'){
        if(*charptr == '.'){checkDec = 1;}
        senddata[senddataCount] = *charptr;
        charptr++;
        senddataCount++;
    }
    if(checkDec == 0){
        senddata[senddataCount] = '.';
        senddata[senddataCount+1] = '0';
        senddataCount = senddataCount + 2;
    }
    checkDec = 0;
    charptr = s3;
    while(*charptr != '\0'){
        senddata[senddataCount] = *charptr;
        charptr++;
        senddataCount++;
    }
    senddata[senddataCount] == '\0';




    // connecting to server
    host = "localhost";
    port = "8080";
    
    clientfd = Open_clientfd(host, port);

    /* 
    Write your code here.
    Recommend to use the Robust I/O package.
    */
    int Maxline = 1024;
    char buffer[Maxline];
    rio_t robustIO;
    Rio_readinitb(&robustIO, clientfd);

    Rio_writen(clientfd, senddata, strlen(senddata));
    Fputs(buffer, stdout);
    fflush(stdout);
    char result[Maxline];
    char result2[Maxline];
    int n = 0; // Number of Lines
    int dataFindFlag = 0;
    while (n < 100){ // Read at most 100 lines.
        Rio_readlineb(&robustIO, buffer, Maxline);
        if (strcmp(buffer, dataFind) == 0){
            dataFindFlag = 1;
            break;
        }
        n++;
    }
    char* start[10]; // support at most 10 return data   
    int dataNumCount = 0;
    char *strptr;
    Rio_readlineb(&robustIO, buffer, Maxline);
    char *start1 = buffer + 15;
    strptr = start1;
    while(*strptr != '<'){
        strptr++;
    }
    *strptr = '\0';
    printf("%s   ", start1);
    Rio_readlineb(&robustIO, buffer, Maxline);
    char *start2 = buffer + 15;
    strptr = start2;
    while(*strptr != '<'){
        strptr++;
    }
    *strptr = '\0';
    printf("%s\n", start2);
    fflush(stdout);
    
    Close(clientfd);
    exit(0);
}
