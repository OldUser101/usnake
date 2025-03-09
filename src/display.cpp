#include "display.h"

namespace usnake {
    void Display::draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
        int cell_width = width / WIDTH;
        int cell_height = height / HEIGHT;
        int cell_size = MIN(cell_width, cell_height);
    
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_paint(cr);
    
        cairo_set_source_rgb(cr, 1, 0, 0);
    
        for (auto it = snake->begin(); it != snake->end(); ++it)
        {
            SNAKE_CELL s = *it;
            cairo_rectangle(cr, s.x * cell_size, s.y * cell_size, cell_size, cell_size);
        }
    
        cairo_fill(cr);
    
        ATTRIBUTED_POINT ap = snake->food();
    
        cairo_set_source_rgb(cr, 0, 1, 0);    
        cairo_rectangle(cr, ap.x * cell_size, ap.y * cell_size, cell_size, cell_size);
    
        cairo_fill(cr);
    }

    Display::Display(Snake *snake) {
        this->snake = snake;
    }
    
    Display::Display() {

    }
};