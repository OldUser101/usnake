#pragma once

#include "snake.h"
#include "display.h"
#include "timer.h"
#include <gtk/gtk.h>

namespace usnake {
    class Application {
    private:
        static Snake snake;
        static Timer timer;
        static Display display;
        static int score;
        static GtkLabel *status_label;
        static GtkWidget *drawing_area;
        static GtkWidget *pause_button;
    
        // WINDOW ACTIVATION //
        static void activate(GtkApplication *app, gpointer user_data);

        // WINDOW CREATION //
        static GtkWidget* get_main_content();
        static GtkWidget* get_header_bar();
        static GtkWidget* get_popover();

        // CALLBACKS //
        static void option_button_callback(GtkButton *button, gpointer user_data);
        static gboolean key_callback(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data);
        static void new_button_callback(GtkWidget *widget, gpointer data);
        static void draw_callback(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);
        static gboolean timer_callback(gpointer user_data);
        static void pause_button_callback(GtkWidget *widget, gpointer data);

        // GAME CONTROL //
        static void handle_play_pause(gpointer user_data);
        static void new_game();
        static void update_status(const char *message);

    public:
        static int start(int argc, char* argv[]);

        Application();
    };
};