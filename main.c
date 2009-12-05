#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "td4emu.h"
#include "td4_utils.h"
#include "state_machine.h"
#include "td4_decoder.h"

#define DEFAULT_HZ "10"
#define USEC 1000000

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
	GtkWidget *entry;;
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

static struct td4_state *state;

static void reset_labels(void)
{
	gtk_label_set_label(GTK_LABEL(widgets.reg_a), "0000");
	gtk_label_set_label(GTK_LABEL(widgets.reg_b), "0000");
	gtk_label_set_label(GTK_LABEL(widgets.c_flag), "0");
	gtk_label_set_label(GTK_LABEL(widgets.counter), "0x00");
}

static void destroy(GtkWidget *widget, gpointer data)
{
	cleanup_opcode_table();
	cleanup_state(state);

	gtk_main_quit();
}

static u_int8_t convert_bin2dec(int idx)
{
	struct check_btns btns = widgets.bit_btns[idx];
	int i;
	char data[8] = { 0 };
	
 	for (i = 0; i < sizeof(data); i++)
		data[i] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(btns.bit[i])) ? 1 : 0;

	return bin2dec(data, 8);
}

static inline void set_bit_data(char *out, int data)
{
	int i, j;

	for (i = 0, j = 3; i < 4; i++, j--) 
		out[j] = (data >> i) & 1 ? '1' : '0';

}

static void show_register_data(GtkWidget *widget, u_int8_t data)
{
	char tmp[4 + 1] = { 0 };

	set_bit_data(tmp, data);
	gtk_label_set_text(GTK_LABEL(widget), tmp);
}

static void show_carry_flag(GtkWidget *widget, u_int8_t data)
{
	char c[2] = { 0 };
	c[0] = get_carry_flag(state) ? '1' : '0';
	gtk_label_set_text(GTK_LABEL(widget), c);
}

static void show_io_port_data(struct io_ports_btns *btns)
{
	int i, j;
	char data[IO_PORT_BIT_SIZE + 1] = { 0 };

	set_bit_data(data, state->io->out_port);
	for (i = IO_PORT_BIT_SIZE - 1, j = 0; i >= 0; i--, j++)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(btns->bit[i]), data[j] == '1' ? TRUE : FALSE);
}


static void show_data(void)
{
	show_register_data(widgets.reg_a, state->acc->reg_a);
	show_register_data(widgets.reg_b, state->acc->reg_b);
	show_carry_flag(widgets.c_flag, state->flags->carry);
	show_io_port_data(&widgets.out_port_btns);
}

static void read_in_port(void)
{
	int i;
	char data[IO_PORT_BIT_SIZE] = { 0 };

	for (i = 0; i < sizeof(data); i++)
		data[i] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets.in_port_btns.bit[i])) ? 1 : 0;

	state->io->in_port = bin2dec(data, IO_PORT_BIT_SIZE);
	
}

static unsigned int get_hz_setting_from_entry(void)
{
	const gchar *text = gtk_entry_get_text(GTK_ENTRY(widgets.clock.entry));
	unsigned int ret = 0;
	int len = strlen(text);

	if (len != strspn(text, "0123456789"))
		return 0;

	ret = (unsigned int) atoi(text);
	if (ret == 0 || ret > 1000)
		return 0;

	return ret;
}

static unsigned int get_hz_setting(void)
{
	unsigned int ret = 0;

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets.clock.radio[0]))) {
		ret = 1;
	} else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets.clock.radio[1]))) {
		ret = 10;
	} else {
		ret = get_hz_setting_from_entry();
	}

	return (ret != 0) ? USEC / ret : 0;

}

static gboolean on_execute(GtkWidget *widget, gpointer data)
{
	int i = 0;
	unsigned int usec;

	// get HZ setting.
	usec = get_hz_setting();
	if (!usec) {
		GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(widgets.window),
				       GTK_DIALOG_MODAL,
				       GTK_MESSAGE_WARNING,
				       GTK_BUTTONS_CLOSE,
				       "HZ should be between 1 to 1000");

		gtk_dialog_run(GTK_DIALOG(d));
		gtk_widget_destroy(d);
		return FALSE ;
	}

	// reset labels
	reset_labels();

	g_print("HZ setting is %uHZ\n", usec);

	// first, read all memory data.
	for (i = 0; i < ADDRESS_SPACE_SIZE; i++) 
		state->memory[i] = convert_bin2dec(i);

	// read IN port data.
	read_in_port();

	// decode and execute instructions.
	while (get_ip(state) < ADDRESS_SPACE_SIZE) {
		parse_opecode(state, fetch(state));

		g_print("A:0x%02x B:0x%02x C:0x%02x\nPC:0x%02x\nIN:0x%02x OUT:0x%02x\n",
			state->acc->reg_a, state->acc->reg_b, state->flags->carry,
			get_ip(state),
			state->io->in_port, state->io->out_port);

		show_data();
		usleep(usec);
	}       
	
	reset_state(state);

	return TRUE;
}

static gboolean on_reset(GtkWidget *widget, gpointer data)
{
	int i, j;

	// reset TD4 registers, carry flag, IP, and memory.
	reset_state(state);

	// reset io port
	for (i = 0; i < IO_PORT_BIT_SIZE; i++) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets.in_port_btns.bit[i]), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets.out_port_btns.bit[i]), FALSE);
	}

	// reset memory
	for (i = 0; i < ADDRESS_SPACE_SIZE; i++) {
		for (j = 7; j >= 0; j--) 
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets.bit_btns[i].bit[j]), FALSE);
	}

	// reset clock.
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets.clock.radio[0]), TRUE);
	gtk_entry_set_text(GTK_ENTRY(widgets.clock.entry), DEFAULT_HZ);

	// reset labels
	reset_labels();

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

	widgets.clock.radio[0] = gtk_radio_button_new_with_label(group, "1HZ");
	widgets.clock.radio[1] = gtk_radio_button_new_with_label_from_widget(
		GTK_RADIO_BUTTON(widgets.clock.radio[0]), "10HZ");

	widgets.clock.radio[2] = gtk_radio_button_new_with_label_from_widget(
		GTK_RADIO_BUTTON(widgets.clock.radio[1]), "Manual");

	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);	
	gtk_box_pack_start(GTK_BOX(vbox), widgets.clock.radio[0], FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), widgets.clock.radio[1], FALSE, FALSE, 0);


	label = gtk_label_new("HZ");
	widgets.clock.entry = gtk_entry_new_with_max_length(4);
	gtk_entry_set_text(GTK_ENTRY(widgets.clock.entry), DEFAULT_HZ);
	gtk_widget_set_size_request(widgets.clock.entry, 30, 20);	

	gtk_box_pack_start(GTK_BOX(hbox), widgets.clock.radio[2], FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), widgets.clock.entry, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
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

	// initialize td4 registers and data
	init_opcode_table();
	state = init_state();

	gtk_main();

}

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);

	create_window();

	return 0;
}

