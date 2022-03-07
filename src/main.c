#include "logger.h"

int main(int argc, char **argv) {
    log_info("Test info");
    log_debug("Test debug");
    log_error("Test error");
    log_warn("Test warn");
    return 0;
}
