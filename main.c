#include <gtk/gtk.h>

#include "td4emu.h"

static void create_window(void)
{
	GtkWidget *window;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window, 300, 200);

	gtk_widget_show_all(window);
	
	gtk_main();

}

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);

	create_window();

	return 0;
}

