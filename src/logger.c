
#include <stdio.h>
#include <malloc.h>
#include "logger.h"
#include "colors.h"
#include "time.h"


char* get_current_date(){
    time_t now;
    time(&now);

    struct tm *local = localtime(&now);
    char* date_string = (char*)malloc(20 * sizeof(char));

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

void log_msg(LogLevel level, char* msg){
    printf("[%s]", get_current_date());
    printf("%s", get_level_color(level));
    printf("[%s]", get_level_str(level));
    printf("%s", C_DEFAULT);
    printf("%s\n", msg);
}


void log_info(char* msg){
    log_msg(LogLevel_Information, msg);
}
void log_error(char* msg){
    log_msg(LogLevel_Error, msg);
}
void log_warn(char* msg){
    log_msg(LogLevel_Warn, msg);
}
void log_debug(char* msg){
    log_msg(LogLevel_Debug, msg);
}