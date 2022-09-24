// TODO: Eliminate possible overlap with legitimate processes

#define PROC_COUNT 7

const char *KILL_PROC[PROC_COUNT] = {
    "tor",     "/usr/bin/tor",   "Telegram", "element-desktop",
    "firefox", "signal-desktop", "veracrypt"};
