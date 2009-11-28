#include <gtk/gtk.h>

#include "td4emu.h"

enum {
	IN_PORT = 0,
	OUT_PORT,
};

struct check_btns {
	GtkWidget *bit[8];
};

struct io_ports_btns {
	GtkWidget *bit[IO_PORT_BIT_SIZE];
};

struct clock_info {
	GtkWidget *radio[3];
	gchar *clock;
};

struct td4_info_widgets {
	GtkWidget *window;
	GtkWidget *reg_a; // show register A's status
	GtkWidget *reg_b; // show register B's status
	GtkWidget *c_flag; // show carry flag's status
	GtkWidget *counter; // Program Counter
	struct check_btns bit_btns[ADDRESS_SPACE_SIZE]; 
	struct io_ports_btns in_port_btns;
	struct io_ports_btns out_port_btns;
	struct clock_info clock;
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

static gboolean on_set_clock(GtkWidget *widget, gpointer data)
{
	show_dialog(widget, "on set clock");
	return TRUE;
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

GtkWidget *create_io_ports_widget(struct io_ports_btns *btns, gboolean enable, const gchar *msg)
{
	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
	GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
	GtkWidget *ret = gtk_hbox_new(FALSE, 0);
	GtkWidget *label = gtk_label_new(msg);

	GtkWidget *bit_label = gtk_label_new("3  2  1  0");
	int i;

	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), bit_label, FALSE, FALSE, 0);

	for (i = IO_PORT_BIT_SIZE - 1; i >= 0; i--) {
		btns->bit[i] = gtk_check_button_new();
		if (!enable)
			gtk_widget_set_sensitive(GTK_WIDGET(btns->bit[i]), FALSE);

		gtk_box_pack_start(GTK_BOX(hbox), btns->bit[i], FALSE, FALSE, 0);
	}

	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(ret), vbox, FALSE, FALSE, 0);
	return ret;
}

static void create_io_ports_btns(GtkWidget *pane)
{
	GtkWidget *hbox_in, *hbox_out;

	hbox_in = create_io_ports_widget(&widgets.in_port_btns, TRUE, "IN PORT");
	hbox_out = create_io_ports_widget(&widgets.out_port_btns, FALSE, "OUT PORT");

	gtk_box_pack_start(GTK_BOX(pane), hbox_out, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(pane), hbox_in, FALSE, FALSE, 0);
}

static void create_bit_btns(GtkWidget *pane)
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
			widgets.bit_btns[i].bit[j] = gtk_check_button_new();
			gtk_box_pack_start(GTK_BOX(hbox), widgets.bit_btns[i].bit[j], FALSE, FALSE, 0);
		}
		gtk_box_pack_start(GTK_BOX(hbox), create_addr_label(i), FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(pane), hbox, FALSE, FALSE, 0);
	}
}

static void create_registers_info(GtkWidget *pane)
{
	GtkWidget *reg_a_str, *reg_b_str, *c_flag_str, *counter_str;
	GtkWidget *hbox;
	GtkWidget *msg_label;
	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);

	// create labels
	msg_label = gtk_label_new("Registers info\n");
	reg_a_str = gtk_label_new("Register A:");
	reg_b_str = gtk_label_new("Register B:");
	c_flag_str = gtk_label_new("Carry:");
	counter_str = gtk_label_new("Program Counter:");

	widgets.reg_a = gtk_label_new("0000");
	widgets.reg_b = gtk_label_new("0000");
	widgets.c_flag = gtk_label_new("0");
	widgets.counter = gtk_label_new("0x00");

	gtk_box_pack_start(GTK_BOX(vbox), msg_label, FALSE, FALSE, 0);

	hbox = gtk_hbox_new(FALSE, 0); 
	gtk_box_pack_start(GTK_BOX(hbox), reg_a_str, FALSE, FALSE, 10);
	gtk_box_pack_end(GTK_BOX(hbox), widgets.reg_a, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);

	hbox = gtk_hbox_new(FALSE, 0); 
	gtk_box_pack_start(GTK_BOX(hbox), reg_b_str, FALSE, FALSE, 10);
	gtk_box_pack_end(GTK_BOX(hbox), widgets.reg_b, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);

	hbox = gtk_hbox_new(FALSE, 0); 
	gtk_box_pack_start(GTK_BOX(hbox), c_flag_str, FALSE, FALSE, 10);
	gtk_box_pack_end(GTK_BOX(hbox), widgets.c_flag, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);

	hbox = gtk_hbox_new(FALSE, 0); 
	gtk_box_pack_start(GTK_BOX(hbox), counter_str, FALSE, FALSE, 10);
	gtk_box_pack_end(GTK_BOX(hbox), widgets.counter, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);

	gtk_box_pack_start(GTK_BOX(pane), vbox, FALSE, FALSE, 5);
}

static void create_clock_box(GtkWidget *pane)
{
	GSList *group = NULL;
	GtkWidget *label = gtk_label_new("\nClock Generator");
	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
	GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
	GtkWidget *set_btn;

	widgets.clock.radio[0] = gtk_radio_button_new_with_label(group, "1HZ");
	widgets.clock.radio[1] = gtk_radio_button_new_with_label_from_widget(
		GTK_RADIO_BUTTON(widgets.clock.radio[0]), "10HZ");

	widgets.clock.radio[2] = gtk_radio_button_new_with_label_from_widget(
		GTK_RADIO_BUTTON(widgets.clock.radio[1]), "Manual");

	set_btn = gtk_button_new_with_label("Set Clock");
	g_signal_connect (G_OBJECT (set_btn), "clicked",
			  G_CALLBACK (on_set_clock), NULL);

	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);	
	gtk_box_pack_start(GTK_BOX(vbox), widgets.clock.radio[0], FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), widgets.clock.radio[1], FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(hbox), widgets.clock.radio[2], FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), set_btn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(pane), vbox, FALSE, FALSE, 0);
}

static void create_window(void)
{
	GtkWidget *hbox;
	GtkWidget *rvbox, *lvbox;
	GtkWidget *reset, *execute;


	// Window default setting.
	widgets.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(widgets.window, 500, 450);
	gtk_window_set_title (GTK_WINDOW(widgets.window), "TD4 Emulator");
	g_signal_connect (G_OBJECT (widgets.window), "destroy",
			  G_CALLBACK (destroy), NULL);

	// create pane.
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(widgets.window), hbox);

	// create execute btns.
	reset = gtk_button_new_with_label("reset");
	g_signal_connect(G_OBJECT(reset), "clicked",
			 G_CALLBACK(on_reset), widgets.window);

	execute = gtk_button_new_with_label("execute");
	g_signal_connect(G_OBJECT(execute), "clicked",
			 G_CALLBACK(on_execute), widgets.window);

	lvbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), lvbox, FALSE, TRUE, 20);
	create_registers_info(lvbox);

	create_io_ports_btns(lvbox);

	create_clock_box(lvbox);

	gtk_box_pack_end(GTK_BOX(lvbox), execute, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(lvbox), reset, FALSE, FALSE, 0);	

	rvbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), rvbox, FALSE, FALSE, 0);
	create_bit_btns(rvbox);
	
	gtk_widget_show_all(widgets.window);
	
	gtk_main();

}

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);

	create_window();

	return 0;
}

