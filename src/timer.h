#pragma once

#include <gtk/gtk.h>

namespace usnake {
    class Timer {
    private:
        int timer_id = 0;
        guint interval = 100;
        GSourceFunc callback;
        gpointer data;

    public:
        void start();
        void stop();
        Timer(guint interval, GSourceFunc callback, gpointer data);
        Timer();
    };
};
