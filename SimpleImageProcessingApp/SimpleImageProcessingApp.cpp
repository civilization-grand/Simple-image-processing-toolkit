
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

uint8_t* ReadPGM(const char* FileName,  int* Weidth, int* Height, int* MaxGray, char Type[])
{
    FILE* F;
    fopen_s(&F, FileName, "rb");

    if (F == NULL)
    {
        printf("Failed to open file: %s\n", FileName);
        return NULL;
    }

    fscanf_s(F, "%s", Type, 3);
    fscanf_s(F, "%d %d", Weidth, Height);
    fscanf_s(F, "%d", MaxGray, 3);
    //fgetc(F);

    int Size = (*Weidth) * (*Height);
    int SizeAligned = (Size + 31) & ~31;

    uint8_t* Buffer = (uint8_t*)_aligned_malloc(SizeAligned, 32);

    fread(Buffer, sizeof(uint8_t), SizeAligned, F);

    fclose(F);

    return Buffer;
}

void WritePGM(const char* FileName, int Weidth, int Height, int MaxGray, char Type[], uint8_t* Buffer)
{
    FILE* F;
    fopen_s(&F, FileName, "wb");

    if (F == NULL)
    {
        printf("Failed to open file: %s\n", FileName);
    }

    fprintf(F, "%s\n", Type);
    fprintf(F, "%d %d\n", Weidth, Height);
    fprintf(F, "%d\n", MaxGray);

    fwrite(Buffer, sizeof(uint8_t), Weidth * Height, F);

    fclose(F);
}

void PrintPGMInfo(int* Weidth, int* Height, int* MaxGray, char Type[])
{
    printf("Weidth: %d\n", Weidth);
    printf("Height: %d\n", Height);
    printf("MaxGray: %d\n", MaxGray);
    printf("Type: %s\n", Type);
}

extern "C" void PGMImageInvert(uint8_t* ResultBuffer, uint8_t* Buffer, int Size);
extern "C" void PGMImageBright(uint8_t* ResultBuffer, uint8_t* Buffer, int Size, int BrightFactor);
extern "C" void PGMImageThreshold(uint8_t* ResultBuffer, uint8_t* Buffer, int Size);

int main()
{
    const char* FileName = "karsten-winegeart-ofdzDVV5l7I-unsplash-_1_.pgm";

    int Weidth, Height, MaxGray;
    char Type[3];

    uint8_t* Buffer = ReadPGM(FileName, &Weidth, &Height, &MaxGray, Type);

    int Size = Weidth * Height;
    int SizeAligned = (Size + 31) & ~31;

    uint8_t* ResultBuffer = (uint8_t*)_aligned_malloc(SizeAligned, 32);

    if (ResultBuffer == NULL)
    {
        printf("Something wrong...\n");
        return 0;
    }

    
    PGMImageInvert(ResultBuffer, Buffer, SizeAligned);
    WritePGM("Test1.pgm", Weidth, Height, MaxGray, Type, ResultBuffer);

    int BrightFactor = 89;

    PGMImageBright(ResultBuffer, Buffer, SizeAligned, BrightFactor);
    WritePGM("Test2.pgm", Weidth, Height, MaxGray, Type, ResultBuffer);
        
    PGMImageThreshold(ResultBuffer, Buffer, SizeAligned);
    WritePGM("Test3.pgm", Weidth, Height, MaxGray, Type, ResultBuffer);

    return 0;
}
