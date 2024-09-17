#include "parse.h"

u8 getRegister(char c){
    switch(c){
        case 'a':
            return (u8) A;
        case 'b':
            return (u8) B;
        case 'c':
            return (u8) C;
        case 'd':
            return (u8) D;
        case 'l':
            return (u8) L;
        case 'h':
            return (u8) H;
        default:
            printf("Register Argument not Found");
            exit(1);
    }
}

void readArgs(struct Token** tok, int tokCount, int* count, int* lineCount){
    char* src;

    if(tok[tokCount]->children == NULL){
        printf("Unallocated Children Tokens\n");
        exit(1);
    }
    for(int i = 0; i < tok[tokCount]->childNum; i++){
        if(tok[tokCount]->children[i] == NULL) tok[tokCount]->children[i] = malloc(sizeof(struct Token));

        src = strtok(NULL, " \t,:");
        if(src == NULL){
            perror("Insufficient Amount of Arguments");
            exit(1);
        }
        if(tok[tokCount]->children[i] == NULL){
            printf("Failed to Allocate Arguments Token\n");
            exit(1);
        }
        count++;

        tok[tokCount]->children[i]->childNum = 0;

        if(strncmp(src, "a", 1) == 0 || strncmp(src, "b", 1) == 0 || strncmp(src, "c", 1) == 0 || strncmp(src, "d", 1) == 0){
            tok[tokCount]->children[i]->data = getRegister(src[0]);
            tok[tokCount]->children[i]->childNum = 0;
            tok[tokCount]->children[i]->type = TK_REG;
            tok[tokCount]->children[i]->src = *lineCount;
            //tok[tokCount]->children[i]->number = *count;
        }
        else if(src[0] == '('){
            tok[tokCount]->children[i]->type = TK_EQ;
            tok[tokCount]->children[i]->src = *lineCount;
            //tok[tokCount]->children[i]->number = *count;
            tok[tokCount]->children[i]->srcData = src;
            tok[tokCount]->childNum = 0;
        }
        else{
            tok[tokCount]->children[i]->data = (u8)atoi(src);
            tok[tokCount]->children[i]->childNum = 0;
            tok[tokCount]->children[i]->type = TK_CONST;
            tok[tokCount]->children[i]->src = *lineCount;
            //tok[tokCount]->children[i]->number = *count;
        }
    }
}

void parse(FILE* asmFile, struct Token* program){
    char s[256];
    int com_res;
    program->type = TK_ROOT; program->number = 0;
    int num_tk = 0;
    int lineCount = 0;

    while(fgets(s, sizeof(s), asmFile)){
        if(strcmp(s, "\n") == 0){
            lineCount++;
            continue;
        }
        s[strcspn(s, "\n")] = 0;
        char* tok = strtok(s, " \t");
        if(tok == NULL){
            perror("Null Token");
            exit(1);
        }
    
        lineCount++;

        if(strcmp(tok, "fnc") == 0){
            num_tk += 1;

            if(!strcmp(strtok(NULL, " \t"), "_start")){
                printf("First Function Listed is not \"_start\"");
                exit(1);
            }
        }
        memset(s, 0, sizeof(s));
    }
    program->childNum = num_tk;
    program->children = malloc(sizeof(struct Token*) * program->childNum);
    for(int i = 0; i < program->childNum; i++){
        program->children[i] = malloc(sizeof(struct Token));
        if(program->children[i] == NULL){
            perror("Failed to initialize function token\n");
            exit(1);
        }
        program->children[i]->number = i;
        program->children[i]->type = TK_FUNC;
    }

    int tokPoint = -1;
    char* funcTokPoint;
    rewind(asmFile);
    while(fgets(s, sizeof(s), asmFile)){
        funcTokPoint = strtok(s, " \t,:");
        if (funcTokPoint == NULL) continue;
        if(!strcmp(funcTokPoint, "fnc")){
            if(tokPoint < program->childNum-1) tokPoint++;
            funcTokPoint = strtok(NULL, " \t,:");
            if(funcTokPoint == NULL) continue;
            program->children[tokPoint]->srcData = malloc(sizeof(char)*(strlen(funcTokPoint)+1));
            strcpy(program->children[tokPoint]->srcData, funcTokPoint);
            
        }
    }
    rewind(asmFile);

    lineCount = 0; int lineAux, tk_count = -1, fnc_count = 0;
    while(fgets(s, sizeof(s), asmFile)){
        if(strcmp(s, "\n") == 0){
            lineCount++;
            continue;
        }
        lineAux = strlen(s);

        s[strcspn(s, "\n")] = 0;
        char* tok = strtok(s, " \t");
        if(tok == NULL){
            perror("Null Token");
            exit(1);
        }

        if((strcmp(tok, "fnc") == 0 || feof(asmFile)) && tk_count != -1){
            program->children[tk_count]->src = lineCount;
            program->children[tk_count]->childNum = fnc_count;
            program->children[tk_count]->children = malloc(sizeof(struct Token*) * program->children[tk_count]->childNum);

            for(int i = 0; i < program->children[tk_count]->childNum; i++){
                program->children[tk_count]->children[i] = malloc(sizeof(struct Token));

                if(program->children[tk_count]->children[i] == NULL){
                    perror("Memory Allocation for Abstract Syntax Tree Failed");
                    exit(1);
                }
            }

            program->children[tk_count]->number = tk_count;
            tk_count++;
            fnc_count = 0;
        }
        lineCount += lineAux;

        if(strcmp(tok, "fnc") == 0 && tk_count == -1){
            tk_count++;
            fnc_count = 0;
            continue;
        }
        fnc_count++;
    }

    int count = tk_count;
    tk_count = 0, fnc_count = -1;
    enum Op op;
    char c;
    rewind(asmFile);
    lineCount = 0;

    while(fgets(s, sizeof(s), asmFile)){
        char* tok = strtok(s, " \t,:");
        if(tok == NULL) continue;
        if(strcmp(tok, "fnc") == 0){
            tk_count = 0; fnc_count++;
            continue;
        }
        if(fnc_count == -1) continue;

        if(strcmp(tok, "hlt") == 0 || 
        strcmp(tok, "rst") == 0 ||
        strcmp(tok, "scn") == 0){
            if(strcmp(tok, "hlt") == 0) op = HALT;
            else if(strcmp(tok, "rst") == 0) op = RESET;
            else{
                op = SCREEN;
            }

            program->children[fnc_count]->children[tk_count]->childNum = 0;
            program->children[fnc_count]->children[tk_count]->data = op;
            program->children[fnc_count]->children[tk_count]->type= TK_OP;
            program->children[fnc_count]->children[tk_count]->number = count;

            if(strtok(NULL, " \t,:") != NULL){
                printf("Incorrect Number of Arguments on line %d\n", lineCount);
                exit(1);
            }
        }
            
        else if(strcmp(tok, "inp") == 0 ||
        strcmp(tok, "jmp") == 0 ||
        strcmp(tok, "wrt") == 0 ||
        strcmp(tok, "ldc") == 0){
            if(strcmp(tok, "inp") == 0) op = INPUT;
            else if(strcmp(tok, "wrt") == 0) op = WRITE;
            else if(strcmp(tok, "jmp") == 0) op = JUMP;
            else{
                op = LOADC;
            }

            program->children[fnc_count]->children[tk_count]->childNum = 1;
            program->children[fnc_count]->children[tk_count]->children = malloc(sizeof(struct Token*)*1);
            program->children[fnc_count]->children[tk_count]->data = op;
            program->children[fnc_count]->children[tk_count]->type= TK_OP;
            program->children[fnc_count]->children[tk_count]->number = count;
            program->children[fnc_count]->children[tk_count]->src = lineCount;
            if(!strcmp(tok, "jmp")){
                program->children[fnc_count]->children[tk_count]->srcData = malloc(sizeof(char)*1024);
                strcpy(program->children[fnc_count]->children[tk_count]->srcData, strtok(NULL, " \t,:"));
                program->children[fnc_count]->children[tk_count]->childNum = 0;
                free(program->children[fnc_count]->children[tk_count]->children);
            }
            else{
                program->children[fnc_count]->children[tk_count]->srcData = s;
                readArgs(program->children[fnc_count]->children, tk_count, &count, &lineCount);
            }
            count++;
        }

        else if(strcmp(tok, "not") == 0 || strcmp(tok, "mov") == 0 || strcmp(tok, "ldm") == 0){
            if(strcmp(tok, "not") == 0) op = NOT;
            else if(strcmp(tok, "ldm") == 0) op = LOADM;
            else{
                op = LOADI;
            }

            program->children[fnc_count]->children[tk_count]->children = malloc(sizeof(struct Token*)*2);
            program->children[fnc_count]->children[tk_count]->childNum = 2;
            program->children[fnc_count]->children[tk_count]->data = op;
            program->children[fnc_count]->children[tk_count]->type= TK_OP;
            program->children[fnc_count]->children[tk_count]->number = count;
            program->children[fnc_count]->children[tk_count]->src = lineCount;
            count++;

            readArgs(program->children[fnc_count]->children, tk_count, &count, &lineCount);
        }

        else if(strcmp(tok, "add") == 0 || strcmp(tok, "and") == 0 || strcmp(tok, "oro") == 0){
            if(strcmp(tok, "add") == 0) op = ADD;
            else if(strcmp(tok, "and") == 0) op = AND;
            else{
                op = OR;
            }

            program->children[fnc_count]->children[tk_count]->children = malloc(sizeof(struct Token*)*3);
            program->children[fnc_count]->children[tk_count]->childNum = 3;
            program->children[fnc_count]->children[tk_count]->data = op;
            program->children[fnc_count]->children[tk_count]->type= TK_OP;
            program->children[fnc_count]->children[tk_count]->number = count;
            program->children[fnc_count]->children[tk_count]->src = lineCount;
            count++;

            readArgs(program->children[fnc_count]->children, tk_count, &count, &lineCount);
        }
        else{
            continue;
        }
            
        tok = strtok(NULL, " \t,:");
        count++;
        tk_count++;
        lineCount++;
    }
}

char* parseFolder(char* path){
    #if defined(_WIN32) || defined(_WIN64)
    char* bin = "bin\\";
    #else
    char* bin = "bin/";
    #endif
    int folderIdx = 0;
    char aux[strlen(path)+1]; strcpy(aux, path);
    char* tok1 = strtok(aux, "/\\");
    char* tok2 = strtok(NULL, "/\\");

    while(tok2 != NULL){
        tok1 = tok2;
        tok2 = strtok(NULL, "/\\");
    }

    char* ret = malloc((strlen(bin)+strlen((tok1))+1));

    if(ret == NULL){
        perror("Unable to Allocate Memory While Parsing Folder Information");
        exit(1);
    }

    strncpy(ret, bin, strlen(bin)); 
    strncpy((ret+strlen(bin)), tok1, strlen(tok1));
    strncpy((ret+strlen(bin)+strlen(tok1)-4), ".bin", strlen(".bin"));

    return ret;
}