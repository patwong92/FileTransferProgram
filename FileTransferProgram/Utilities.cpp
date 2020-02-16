#include "Utilities.h"

int OpenFileToRead(FILE** read_file, char* read_path)
{
    if ((*read_file = fopen(read_path, "r")) == NULL)
        return 1;

    return 0;
}

int OpenFileToWrite(FILE** read_file, char* write_path)
{
    if ((*read_file = fopen(write_path, "w")) == NULL)
        return 1;

    return 0;
}

int ReadFile(char* read_path, char** content)
{
    FILE* read_file;
    char read_buffer[DATA_BUFSIZE];
    long length;

    if (OpenFileToRead(&read_file, read_path) != 0)
    {
        OutputDebugString("Error in opening file\n");
        return 1;
    }

    //Seek to end of file to get the length
    if (fseek(read_file, 0L, SEEK_END) != 0)
    {
        OutputDebugString("Error in seeking to end\n");
        fclose(read_file);
        return 1;
    }

    length = ftell(read_file);
    char length_string[10];
    _itoa(length, length_string, sizeof(length_string));
    OutputDebugString(length_string);

    //Seek to beginning to file to write
    if (fseek(read_file, 0L, SEEK_SET) != 0)
    {
        OutputDebugString("Error in seeking to beginning\n");
        fclose(read_file);
        return 1;
    }

    memset(read_buffer, 0, sizeof(read_buffer));
    fread(&read_buffer, (length + 1), sizeof(char), read_file);

    //Truncate garbage characters here
    *content = read_buffer;

    CloseFile(&read_file);

    return 0;
}

int WriteFile(char* write_path, char** write_buf)
{
    FILE* write_file;

    if (OpenFileToWrite(&write_file, write_path) != 0)
    {
        perror("Error in opening file\n");
        return 1;
    }

    if (fseek(write_file, 0L, SEEK_END) != 0)
    {
        printf("Error in seeking!\n");
        fclose(write_file);
        return 1;
    }

    fwrite(*write_buf, 1, strlen(*write_buf) + 1, write_file);
    
    CloseFile(&write_file);

    return 0;
}

int CloseFile(FILE** file)
{
    if (fclose(*file) != 0)
        return 1;

    return 0;
}
