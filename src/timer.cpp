#include "timer.h"

namespace usnake {
    void Timer::start() {
        timer_id = g_timeout_add(interval, callback, data);
    }

    void Timer::stop() {
        if (timer_id != 0) {
            g_source_remove(timer_id);
            timer_id = 0;
        }
    }

    Timer::Timer(guint interval, GSourceFunc callback, gpointer data) {
        this->interval = interval;
        this->callback = callback;
        this->data = data;
    }

    Timer::Timer() {

    }
};