#include <gtk/gtk.h>

#include "td4emu.h"

struct check_buttons {
	GtkWidget *bit[8];
};
static struct check_buttons bit_buttons[ADDRESS_SPACE_SIZE]; 

struct td4_info_widgets {
	GtkWidget *window;
	GtkWidget *reg_a; // show register A's status
	GtkWidget *reg_b; // show register B's status
	GtkWidget *c_flag; // show carry flag's status
};
static struct td4_info_widgets widgets;

static void show_dialog(GtkWidget *widget, gchar *msg)
{
	GtkWidget *dialog;

	dialog = gtk_message_dialog_new(GTK_WINDOW(widgets.window),
					GTK_DIALOG_MODAL,
					GTK_MESSAGE_INFO,
					GTK_BUTTONS_OK,
					msg);

	gtk_dialog_run(GTK_DIALOG(dialog));

	gtk_widget_destroy(dialog);

}

static void destroy(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}


static gboolean on_execute(GtkWidget *widget, gpointer data)
{
	show_dialog(widget, "on execute");
	gtk_label_set_text(GTK_LABEL(widgets.reg_a), "AAAA");
	return TRUE;
}

static gboolean on_reset(GtkWidget *widget, gpointer data)
{
	show_dialog(widget, "on reset");
	return TRUE;
}

	
static GtkWidget *create_addr_label(int count)
{
	gchar *tmp = g_strdup_printf("Address 0x%02x", count);
	GtkWidget *ret;

	ret = gtk_label_new(tmp);

	g_free(tmp);
	tmp = NULL;

	return ret;
}

static void create_bit_buttons(GtkWidget *pane)
{
	int i, j;
	GtkWidget *hbox;
	GtkWidget *label;

	hbox = gtk_hbox_new(FALSE, 0);

	label = gtk_label_new("           Program Memory\nbit7                                bit0");
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(pane), hbox, FALSE, FALSE, 0);

	for (i = 0; i < ADDRESS_SPACE_SIZE; i++) {
		hbox = gtk_hbox_new(FALSE, 0);

		for (j = 7; j >= 0; j--) {
			bit_buttons[i].bit[j] = gtk_check_button_new();
			gtk_box_pack_start(GTK_BOX(hbox), bit_buttons[i].bit[j], FALSE, FALSE, 0);
		}
		gtk_box_pack_start(GTK_BOX(hbox), create_addr_label(i), FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(pane), hbox, FALSE, FALSE, 0);
	}
}

	
static void create_window(void)
{
	GtkWidget *window;
	GtkWidget *hbox;
	GtkWidget *rvbox, *lvbox;
	GtkWidget *reset, *execute;
	GtkWidget *reg_a, *reg_b, *c_flag;
	GtkWidget *reg_a_str, *reg_b_str, *c_flag_str;
	GtkWidget *hbox_tmp;

	// Window default setting.
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window, 450, 400);

	g_signal_connect (G_OBJECT (window), "destroy",
			  G_CALLBACK (destroy), NULL);

	// create pane.
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), hbox);

	// create execute buttons.
	reset = gtk_button_new_with_label("reset");
	g_signal_connect(G_OBJECT(reset), "clicked",
			 G_CALLBACK(on_reset), window);

	execute = gtk_button_new_with_label("execute");
	g_signal_connect(G_OBJECT(execute), "clicked",
			 G_CALLBACK(on_execute), window);

	// create labels
	reg_a_str = gtk_label_new("Register A:");
	reg_b_str = gtk_label_new("Register B:");
	c_flag_str = gtk_label_new("Carry:");
	reg_a = gtk_label_new("0000");
	reg_b = gtk_label_new("0000");
	c_flag = gtk_label_new("0");

	lvbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), lvbox, FALSE, TRUE, 20);

	hbox_tmp = gtk_hbox_new(FALSE, 0); 
	gtk_box_pack_start(GTK_BOX(hbox_tmp), reg_a_str, FALSE, FALSE, 20);
	gtk_box_pack_end(GTK_BOX(hbox_tmp), reg_a, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(lvbox), hbox_tmp, FALSE, FALSE, 5);

	hbox_tmp = gtk_hbox_new(FALSE, 0); 
	gtk_box_pack_start(GTK_BOX(hbox_tmp), reg_b_str, FALSE, FALSE, 20);
	gtk_box_pack_end(GTK_BOX(hbox_tmp), reg_b, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(lvbox), hbox_tmp, FALSE, FALSE, 5);

	hbox_tmp = gtk_hbox_new(FALSE, 0); 
	gtk_box_pack_start(GTK_BOX(hbox_tmp), c_flag_str, FALSE, FALSE, 20);
	gtk_box_pack_end(GTK_BOX(hbox_tmp), c_flag, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(lvbox), hbox_tmp, FALSE, FALSE, 5);

	gtk_box_pack_start(GTK_BOX(lvbox), reset, FALSE, FALSE, 0);	
	gtk_box_pack_start(GTK_BOX(lvbox), execute, FALSE, FALSE, 0);

	rvbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), rvbox, FALSE, FALSE, 0);
	create_bit_buttons(rvbox);
	
	gtk_widget_show_all(window);
	
	widgets.window = window;
	widgets.reg_a = reg_a;
	widgets.reg_b = reg_b;
	widgets.c_flag = c_flag;

	gtk_main();

}

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);

	create_window();

	return 0;
}

