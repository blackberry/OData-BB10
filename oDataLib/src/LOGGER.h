#include <stdio.h>

/**
 * Simple log helper that logs to the Console Window of Momentics
 */
class LOGGER {
public:
    static void log(const char* msg) {
        fprintf(stdout, "%s\n", msg);
        fflush(stdout);
    }

    static void log(const char* msg, const char* value) {
        fprintf(stdout, "%s %s\n", msg, value);
        fflush(stdout);
    }

    static void log(const char* msg, int iValue, const char* value) {
        fprintf(stdout, "%s %d:%s\n", msg, iValue, value);
        fflush(stdout);
    }

    static void log(const char* msg, int value) {
        fprintf(stdout, "%s %d\n", msg, value);
        fflush(stdout);
    }

    static void log(const char* msg, int value, const char* value2, int value3) {
        fprintf(stdout, "%s %d %s%d\n", msg, value, value2, value3);
        fflush(stdout);
    }

    static void log(const char* msg, bool value) {
        fprintf(stdout, "%s %d\n", msg, value);
        fflush(stdout);
    }

    static void log(const QString& msg) {
        fprintf(stdout, "%s\n", msg.toLocal8Bit().data());
        fflush(stdout);
    }

    static void log(const QString& msg, const QString& msg2) {
        fprintf(stdout, "%s %s\n", msg.toLocal8Bit().data(), msg2.toLocal8Bit().data());
        fflush(stdout);
    }
};
