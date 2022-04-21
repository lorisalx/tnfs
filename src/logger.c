
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include "logger.h"
#include "colors.h"
#include "time.h"


char* get_current_date(){
    time_t now;
    time(&now);

    struct tm *local = localtime(&now);
    char* date_string = (char*)malloc(30 * sizeof(char));

    sprintf(date_string, "%02d/%02d/%02d-%02d:%02d:%02d", local->tm_mday, local->tm_mon+1, local->tm_year+1900,
            local->tm_hour, local->tm_min, local->tm_sec);
    return date_string;
}

char* get_level_color(LogLevel level){
    switch (level) {
        case LogLevel_Error:
            return C_RED;
        case LogLevel_Warn:
            return C_YELLOW;
        case LogLevel_Debug:
            return C_BLUE;
        case LogLevel_Information:
            return C_GREEN;
        default:
            return C_DEFAULT;
    }
}

char* get_level_str(LogLevel level){
    switch (level) {
        case LogLevel_Error:
            return "ERR";
        case LogLevel_Warn:
            return "WRN";
        case LogLevel_Debug:
            return "DBG";
        case LogLevel_Information:
            return "INF";
        default:
            return "DBG";
    }
}

void log_msg(LogLevel level, char* format, va_list *args){
    char *date_string = get_current_date();
    printf("[%s]", date_string);
    printf("%s", get_level_color(level));
    printf("[%s]", get_level_str(level));
    printf("%s", C_DEFAULT);
    if(args == NULL)
    {
        printf(format);
    }
    else
    {
        va_list argsCopy;
        va_copy(argsCopy, *args);
        int len = vsnprintf(NULL, 0, format, argsCopy);
        va_end(argsCopy);
        char buffer[len + 1];
        vsnprintf(buffer, sizeof(buffer), format, *args);
        printf(buffer);
    }
    printf("\n");
    fflush(stdout);
    free(date_string);
}

void log_formated_info(char* format, ...){
    va_list args;
    va_start(args, format);
    log_msg(LogLevel_Information, format, &args);
    va_end(args);
}

void log_info(char* format)
{
    log_msg(LogLevel_Information, format, NULL);
}

void log_formated_error(char* format, ...){
    va_list args;
    va_start(args, format);
    log_msg(LogLevel_Error, format, &args);
    va_end(args);
}

void log_error(char* format)
{
    log_msg(LogLevel_Error, format, NULL);
}

void log_formated_warn(char* format, ...){
    va_list args;
    va_start(args, format);
    log_msg(LogLevel_Warn, format, &args);
    va_end(args);
}

void log_warn(char* format){
    log_msg(LogLevel_Warn, format, NULL);
}

void log_formated_debug(char* format, ...){
    va_list args;
    va_start(args, format);
    log_msg(LogLevel_Debug, format, &args);
    va_end(args);
}
void log_debug(char* format){
    log_msg(LogLevel_Debug, format, NULL);
}
