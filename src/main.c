#include <gtk/gtk.h>

//------------------------------------------------------------------
// Custom structure that holds pointers to widgets and user variables

typedef struct {
// Add pointers to widgets below
//------------------------------------------------------------------
// GtkWidget *w_x;
   // GtkWidget *w_window_main;
   GtkWidget *w_aboutWindow; // Pointer to about dialog box
} app_widgets;

int main(int argc, char *argv[])
{

    GtkBuilder      *builder; 
    GtkWidget       *window;    
    app_widgets     *widgets = g_slice_new(app_widgets); // Instantiate structure, allocating memory for it

    gtk_init(&argc, &argv); // init Gtk

//------------------------------------------------------------------
// Establish contacht with xml code user to adjust widget settings
//------------------------------------------------------------------
   
    builder = gtk_builder_new_from_file("glade/window_main.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

//------------------------------------------------------------------
// Get pointers to widgets here
// widgets->w_x  = GTK_WIDGET(gtk_builder_get_object(builder, "x"));
//------------------------------------------------------------------

    widgets->w_aboutWindow  = GTK_WIDGET(gtk_builder_get_object(builder, "aboutWindow"));


    gtk_builder_connect_signals(builder, widgets); // Widgets pointer are passed to all widget handler functions as the user_data parameter

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();
    g_slice_free(app_widgets, widgets); // Free up widget structure memory

    return 0;
}

// Dummy handler function
void x(GtkButton *button, app_widgets *app_wdgts)
{
}


void on_menuQuit_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts){
	printf("Quit program.\n");
	gtk_main_quit();
}


void on_menuAbout_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts){
	gtk_widget_show(app_wdgts->w_aboutWindow);
}

// About dialog box Close button in x and close button
void on_aboutWindow_response(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	gtk_widget_hide(app_wdgts->w_aboutWindow);
}
void on_closeBtn_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	gtk_widget_hide(app_wdgts->w_aboutWindow);
}


// called when window is closed
void on_window_main_destroy()
{
	gtk_main_quit();
}
