#pragma once

#include <gtk/gtk.h>

#include "snake.h"

namespace usnake {
    class Display {
    private:
        Snake *snake;
    
    public:
        void draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);
        Display(Snake *snake);
        Display();
    };
};