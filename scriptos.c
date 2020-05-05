

//
// Created by louda on 4/19/20.
//
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
 
void remove_spaces(char* s) {
    const char* d = s;
    do {
        while (*d == ' ' || *d == '\t' || *d == '\v' || *d == '\f') {
            ++d;
        }
    } while (*s++ = *d++);
}
 
void print(char* this){
    printf("%s\n",this);
    return;
}
 
void err(char* str,char* reason){
    print(str);
    print(reason);
    exit(1);
}
 
void getVals(char* str, int hue[4]){
    char* togen;
    int i = 0;
    togen = strtok(str,":");
    while (togen != NULL){
        if (i > 2){
            hue[i-3] = atoi(togen);
        }
        i++;
        togen = strtok(NULL,":");
    }
    return;
}
 
int main(){
    int judgeIn = 0;
    char line[69];
    char errLine[69];
    int shit[4];
    int doneImm = 0;
    int checkedImm = 0;
    int unchImm = 0;
    int immWant = 0;
    int immGet = 0;
    int immIn = 0;
    int judeWants = 0;
    int leaver = 0;
    int immTemp = 0;
 
 
 
    regex_t immEnters;
    regcomp(&immEnters,"IMM.*enters",0);
    regex_t immLeaves;
    regcomp(&immLeaves,"IMM.*leaves",0);
    regex_t immChecks;
    regcomp(&immChecks,"IMM.*checks",0);
    regex_t immWants;
    regcomp(&immWants,"IMM.*wants.certificate",0);
    regex_t immGets;
    regcomp(&immGets,"IMM.*gets.certificate",0);
 
    regex_t judgeWantEnters;
    regcomp(&judgeWantEnters,"JUDGE.*wants to enter",0);
    regex_t judgeEnters;
    regcomp(&judgeEnters,"JUDGE.*enters",0);
    regex_t judgeLeaves;
    regcomp(&judgeLeaves,"JUDGE.*leaves",0);
    regex_t judgeWaits;
    regcomp(&judgeWaits,"JUDGE.*waits",0);
    regex_t judgeConfirmS;
    regcomp(&judgeConfirmS,"JUDGE.*starts.confirmation",0);
    regex_t judgeConfirm;
    regcomp(&judgeConfirm,"JUDGE.*ends.confirmation",0);
    regex_t judgeFin;
    regcomp(&judgeFin,"JUDGE.*finishes",0);
 
 
    while(fgets(line, 69, stdin)){
        strcpy(errLine,line);
        remove_spaces(line);
        getVals(line,shit);
 
        // immigrant enters
        if (!regexec(&immEnters,errLine,0,NULL,0)){
            if (judgeIn)
                err(errLine,"Immigrant cant enter while judge is inside");
 
            if (shit[0] != ++unchImm) {
                err(errLine, "Spaghetti much");
            }
 
            if (shit[2] != ++immIn)
                err(errLine, "Spaghetti much");
        }
 
        // immigrant checks
        if (!regexec(&immChecks,errLine,0,NULL,0)){
            if (shit[1] != ++checkedImm)
                err(errLine, "Spaghetti much");
        }
 
        // immigrant wants certificate
        if (!regexec(&immWants,errLine,0,NULL,0)){
            if (--immWant < 0)
                err(errLine, "Spaghetti much");
        }
 
        // immigrant gets certificate
        if (!regexec(&immGets,errLine,0,NULL,0)) {
            if (--immGet < 0)
                err(errLine, "Spaghetti much");
        }
 
        // immigrant leaves
        if (!regexec(&immLeaves,errLine,0,NULL,0)){
            if (judgeIn)
                err(errLine,"Immigrant cant leave while judge is inside");
 
            if (shit[2] != --immIn)
                err(errLine, "Spaghetti much");
        }
 
        // judge wants enters
        if (!regexec(&judgeWantEnters,errLine,0,NULL,0)){
            if (++judeWants != 1)
                err(errLine, "Spaghetti much");
        }
 
        // judge enters
        if (!regexec(&judgeEnters,errLine,0,NULL,0)){
            if (--judeWants != 0)
                err(errLine, "Spaghetti much");
            judgeIn = 1;
            leaver = 1;
        }
 
        // judge waits
        if (!regexec(&judgeWaits,errLine,0,NULL,0)){
            if(shit[0] == shit[1])
                err(errLine, "Judge shouldnt wait for 0 immigrants");
        }
 
        // judge confirmsS
        if (!regexec(&judgeConfirmS,errLine,0,NULL,0)){
            if (shit[0] != shit[1])
                err(errLine, "Judge has to wait for immigrants if all not checked");
            immTemp = shit[1];
            if (shit[1] > 0)
                leaver = 0;
        }
 
        // judge confirmsE
        if (!regexec(&judgeConfirm,errLine,0,NULL,0)){
            immGet+= immTemp;
            immWant+= immTemp;
            unchImm = checkedImm = 0;
        }
 
        // judge leaves
        if (!regexec(&judgeLeaves,errLine,0,NULL,0)){
            judgeIn = 0;
        }
 
        // judge finishes
        if (!regexec(&judgeFin,errLine,0,NULL,0)) {
            judgeIn = 0;
            if (leaver)
                err(errLine, "Judge enters building for no reason");
        }
    }
 
    print("Alles gute");
    return 0;

    }

