#ifndef LOGGER_H
#define LOGGER_H
typedef enum LogLevel{
    LogLevel_Error,
    LogLevel_Information,
    LogLevel_Debug,
    LogLevel_Warn
} LogLevel;

void log_formated_info(char*, ...);
void log_info(char*);

void log_formated_error(char*, ...);
void log_error(char*);

void log_formated_warn(char*, ...);
void log_warn(char*);

void log_formated_debug(char*, ...);
void log_debug(char*);

#endif //LOGGER_H
