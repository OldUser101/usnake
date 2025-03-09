#include "application.h"

namespace usnake {
    Snake Application::snake;
    Timer Application::timer;
    Display Application::display;
    int Application::score = 0;
    GtkLabel *Application::status_label = nullptr;
    GtkWidget *Application::drawing_area = nullptr;
    GtkWidget *Application::pause_button = nullptr;

    /****************/
    /* GAME CONTROL */
    /****************/
    void Application::handle_play_pause(gpointer user_data) {
        GtkWidget *widget = (GtkWidget*)user_data;
    
        if (snake.gameover()) {
            return;
        }
    
        GtkWidget *image = gtk_button_get_child(GTK_BUTTON(widget));
        const char *current_icon = gtk_image_get_icon_name(GTK_IMAGE(image));
    
        if (g_strcmp0(current_icon, "media-playback-start") == 0) {
            gtk_image_set_from_icon_name(GTK_IMAGE(image), "media-playback-pause");
            snake.resume_game();
        } else {
            gtk_image_set_from_icon_name(GTK_IMAGE(image), "media-playback-start");
            snake.pause_game();
        }
    }

    void Application::new_game() {
        update_status("Score: 0");
        gtk_widget_set_sensitive(pause_button, true);
        snake.new_game();
    }

    void Application::update_status(const char *message) {
        gtk_label_set_text(status_label, message);
    }



    /*************/
    /* CALLBACKS */
    /*************/
    void Application::draw_callback(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
        display.draw(area, cr, width, height, user_data);
    }

    void Application::pause_button_callback(GtkWidget *widget, gpointer data) {
        handle_play_pause(widget);
    }

    void Application::new_button_callback(GtkWidget *widget, gpointer data) {
        new_game();
    }

    gboolean Application::timer_callback(gpointer user_data) {
        snake.cycle();
    
        if (snake.score() != score) {
             std::string msg = "Score: ";
             msg += std::to_string(snake.score());
             update_status(msg.c_str());   
        }
    
        if (snake.gameover()) {
            update_status("Game Over");
            gtk_widget_set_sensitive(pause_button, false);
        }
    
        GtkWidget *drawing_area = GTK_WIDGET(user_data);
        gtk_widget_queue_draw(drawing_area);
    
        return G_SOURCE_CONTINUE;
    }

    gboolean Application::key_callback(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
        switch (keyval) {
            case GDK_KEY_Up:
                snake.set_direction(UP);
                break;
            case GDK_KEY_Down:
                snake.set_direction(DOWN);
                break;
            case GDK_KEY_Left:
                snake.set_direction(LEFT);
                break;
            case GDK_KEY_Right:
                snake.set_direction(RIGHT);
                break;
            case GDK_KEY_F2:
                new_game();
                break;
            case GDK_KEY_F1:
                handle_play_pause(pause_button);
                break;
            default:
                return GDK_EVENT_PROPAGATE;
            }

        return GDK_EVENT_STOP;
    }

    void Application::option_button_callback(GtkButton *button, gpointer user_data) {
        GtkPopover *popover = GTK_POPOVER(user_data);
        gtk_popover_popup(popover);
    }



    /*******************/
    /* WINDOW CREATION */
    /*******************/
    GtkWidget* Application::get_popover() {
        GtkWidget *popover = gtk_popover_new();
    
        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        GtkWidget *setting = gtk_button_new_with_label("Setting 1");
        gtk_box_append(GTK_BOX(box), setting);
    
        gtk_popover_set_child(GTK_POPOVER(popover), box);
    
        gtk_popover_set_position(GTK_POPOVER(popover), GTK_POS_BOTTOM);
    
        return popover;
    }
    
    GtkWidget* Application::get_header_bar() {
        GtkWidget *header_bar = gtk_header_bar_new();
    
        GtkWidget *new_button = gtk_button_new();
        GtkWidget *icon = gtk_image_new_from_icon_name("list-add-symbolic");
        gtk_button_set_child(GTK_BUTTON(new_button), icon);
        g_signal_connect(new_button, "clicked", G_CALLBACK(new_button_callback), NULL);
        gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), new_button);
    
        GtkWidget *state_icon = gtk_image_new_from_icon_name("media-playback-pause");
        pause_button = gtk_button_new();
        gtk_button_set_child(GTK_BUTTON(pause_button), state_icon);
        g_signal_connect(pause_button, "clicked", G_CALLBACK(pause_button_callback), state_icon);
        gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), pause_button);
    
        GtkWidget *popover = get_popover();
    
        GtkWidget *option_button = gtk_menu_button_new();
        gtk_menu_button_set_popover(GTK_MENU_BUTTON(option_button), popover);
        gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(option_button), "dialog-information-symbolic");
        gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), option_button);
    
        status_label = GTK_LABEL(gtk_label_new("Score: 0"));
    
        GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_box_set_homogeneous(GTK_BOX(header_box), TRUE);
        gtk_box_append(GTK_BOX(header_box), GTK_WIDGET(status_label));
    
        gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header_bar), header_box);
    
        return header_bar;
    }
    
    GtkWidget* Application::get_main_content() {
        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    
        GtkWidget *aspect_frame = gtk_aspect_frame_new(0.5, 0.5, 1.0, TRUE);
        gtk_widget_set_hexpand(aspect_frame, TRUE);
        gtk_widget_set_vexpand(aspect_frame, TRUE);
    
        drawing_area = gtk_drawing_area_new();
        gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(drawing_area), 450);
        gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(drawing_area), 450);
        gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_callback, NULL, NULL);
    
        gtk_aspect_frame_set_child(GTK_ASPECT_FRAME(aspect_frame), drawing_area);
        gtk_box_append(GTK_BOX(vbox), aspect_frame);
    
        return vbox;
    }
    


    /*********************/
    /* WINDOW ACTIVATION */
    /*********************/
    void Application::activate(GtkApplication *app, gpointer user_data) {
        GtkWidget *window = gtk_application_window_new(app);
        gtk_window_set_title(GTK_WINDOW(window), "");
        gtk_window_set_default_size(GTK_WINDOW(window), 450, 450);
    
         GtkEventController *key_controller = gtk_event_controller_key_new();
         g_signal_connect(key_controller, "key-pressed", G_CALLBACK(key_callback), NULL);
         gtk_widget_add_controller(window, key_controller);
    
        GtkWidget *header_bar = get_header_bar();
        GtkWidget *content = get_main_content();
    
        gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);
        gtk_window_set_child(GTK_WINDOW(window), content);
    
        gtk_widget_set_visible(window, TRUE);
    
        timer = Timer(100, timer_callback, drawing_area);
        snake = Snake(&timer);
        display = Display(&snake);
    }

    int Application::start(int argc, char* argv[]) {
        GtkApplication *app = gtk_application_new("org.nathanjgill.uSnake", G_APPLICATION_DEFAULT_FLAGS);
    
        g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
        int status = g_application_run(G_APPLICATION(app), argc, argv);
        g_object_unref(app);
    
        return status;
    }

    Application::Application() {
        return;
    }
};