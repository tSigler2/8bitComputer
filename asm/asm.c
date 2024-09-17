#include "asm.h"

bool verbose = false, reset = true;
char* inFile = NULL;
int ramPointer = 0;

void deallocToken(struct Token* tok){
    if(tok->childNum == 0){
        free(tok);
        return;
    }
    for(int i = 0; i < tok->childNum; i++){
        deallocToken(tok->children[i]);
    }
    free(tok->children);
    free(tok);
}

int main(int argc, char** argv){
    if(argc == 1){
        printf("No input file");
        return 1;
    }

    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-v") == 0) verbose = true;
        else if(strcmp(argv[i], "-r") == 0) reset = false;
        else if (strcmp(argv[i], "./asm.o") == 0) continue;
        else if(inFile == NULL) inFile = argv[i];
    }

    FILE* asmFile = fopen(inFile, "r");

    if(asmFile == NULL){
        printf("File not found");
        return 1;
    }

    char outFile[strlen(inFile)+1];

    strcpy(outFile, inFile);

    if(asmFile == NULL){
        perror("File opening error");
        return 1;
    }

    char* outFile_ = parseFolder(inFile);

    if(outFile_ == NULL){
        printf("Output file failed to generate");
        return 1;
    }
#if defined(_WIN32) || defined(_WIN64)
    _mkdir("bin")
#else
    struct stat st;
    if(stat("bin", &st) != 0){
        if(mkdir("bin", 0755) != 0){
            perror("Error Creating Bin Directory");
            return 1;
        }
    }
#endif

    parse(asmFile, &program);
    fclose(asmFile);

    FILE* binFile = fopen(outFile_, "wb");
    if(binFile == NULL){
        perror("Failed to generate output file\n");
        exit(1);
    }

    if(reset){
        u8 r = (u8) 0x10;
        fwrite(&r, sizeof(u8), 1, binFile);
    }

    generateBinary(&program, binFile);
    for(int i = 0; i < program.childNum; i++) deallocToken(program.children[i]);
    fclose(binFile);

    return 0;
}