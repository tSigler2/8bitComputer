#include "gen.h"

u8 checkRegCombo(struct Token* reg1, struct Token* reg2){
    if(reg1 == reg2){
        if(reg1->data == A) return (u8)0x00;
        if(reg1->data == B) return (u8)0x03;
        if(reg1->data == C) return (u8)0x05;
        else{
            printf("Register Argument Not Recognized\n");
            exit(1);
        }
    }

    if(reg1->data == A && reg2->data == B) return (u8)0x00;
    if(reg1->data == A && reg2->data == C) return (u8)0x01;
    if(reg1->data == A && reg2->data == D) return (u8)0x02;
    if(reg1->data == B && reg2->data == C) return (u8)0x03;
    if(reg1->data == B && reg2->data == D) return (u8)0x04;
    if(reg1->data == C && reg2->data == D) return (u8)0x05;
    else{
        printf("Unrecognized ALU Arguments\n");
        exit(1);
    }
}

u8 checkCommand(struct Token* token){
    u8 byte = token->data;
    switch(byte){
        case HALT:
            return byte;
        case JUMP:
            return byte;
        case ADD:
            byte += checkRegCombo(token->children[0], token->children[1]);
            if(token->children[2]->data == B) return (u8) (byte + 0x08);
            return byte;
        case AND:
            byte += checkRegCombo(token->children[0], token->children[1]);
            if(token->children[2]->data == B) return (u8) (byte + 0x08);
            return byte;
        case OR:
            byte += checkRegCombo(token->children[0], token->children[1]);
            if(token->children[2]->data == B) return (u8) (byte + 0x08);
            return byte;
        case NOT:
            byte += checkRegCombo(token->children[0], token->children[0]);
            if(token->children[1]->data == B) return (u8) (byte + 0x08);
            return byte;
        case LOADI:
            loadImm = true;
            return (u8) (byte + token->children[0]->data);
        case WRITE:
            byte += token->children[0]->data;
            return byte;
        case LOADC:
            if(token->children[0]->data > 0x0f || token->children[0]->data < 0x00){
                printf("Color out of Renderable Range\n");
                exit(1);
            }
            return (u8) (byte + token->children[0]->data);
        case LOADM:
            return (u8) (byte + token->children[0]->data);
        case INPUT:
            return byte;
        case RESET:
            return byte;
        case SCREEN:
            return byte;
        default:
            printf("Unrecognized Command\n");
            exit(1);
    }
}

int findFuncAddress(char** funcAddressList, char* func){
    for(int i = 0; i < ARRLEN(funcAddressList); i++){
        if(strcmp(func, funcAddressList[i])) return i;
    }
    printf("Function Finding Error");
    exit(1);
}

void generateBinary(struct Token* program, FILE* outFile){
    loadImm = false;
    funcTracker = malloc(sizeof(u8)*program->childNum*2);
    funcTracker[0] = 0x00; funcTracker[1] = 0x00;

    char** funcAddresses = malloc(sizeof(char*)*program->childNum);
    char** funcNote = malloc(sizeof(char*)*1024);

    u16 funcIndex[1024] = {0x0000};
    int funcIndexP = 0;

    u16 byteCounter = 0x0000;
    u8 command, jumpAdd;

    for(int i = 0; i < program->childNum; i++){
        funcAddresses[i] = malloc(sizeof(char)*(strlen(program->children[i]->srcData)+1));
        funcAddresses[i] = strcpy(funcAddresses[i], program->children[i]->srcData);
    }
    
    for(int i = 0; i < program->childNum; i++){
        if(i > 0){
            funcTracker[2*i] = (u8)byteCounter;
            funcTracker[2*i+1] = (u8)(byteCounter >> 8);
        }
        if(program->children[i] == NULL){
            printf("Null Token\n");
            exit(1);
        }
        if(program->children[i]->children == NULL) continue;

        for(int j = 0; j < program->children[i]->childNum; j++){
            command = checkCommand(program->children[i]->children[j]);
            if(command == JUMP){
                jumpAdd = (u8)(LOADI+L);
                fwrite(&jumpAdd, sizeof(u8), 1, outFile);
                jumpAdd = (u8)0x00;
                fwrite(&jumpAdd, sizeof(u8), 1, outFile);
                jumpAdd = (u8)(LOADI+H);
                fwrite(&jumpAdd, sizeof(u8), 1, outFile);
                jumpAdd = (u8)0x00;
                fwrite(&jumpAdd, sizeof(u8), 1, outFile);
                funcIndex[funcIndexP] = byteCounter;
                funcNote[funcIndexP] = program->children[i]->children[j]->srcData;
                funcIndexP++;
                byteCounter += 5;
            }
            
            fwrite(&command, sizeof(command), 1, outFile);
            byteCounter++;

            if(loadImm){
                fwrite(&program->children[i]->children[j]->children[0]->data, sizeof(u8), 1, outFile);
                loadImm = false;
                byteCounter++;
            }
        }
    }
    int funcFind;
    u8 high, low;
    for(int i = 0; i < funcIndexP; i++){
        fseek(outFile, (long)funcIndex[i], 0);
        funcFind = findFuncAddress(funcAddresses, funcNote[i]);
        low = funcTracker[2*funcFind];
        high = funcTracker[2*funcFind+1];
        fwrite(&low, sizeof(u8), 1, outFile+1);
        fwrite(&high, sizeof(u8), 1, outFile+3);
    }
    free(funcTracker);
}