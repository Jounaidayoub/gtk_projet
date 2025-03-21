#include <gtk/gtk.h>

static void on_menu_item_clicked(GtkWidget *widget, gpointer data) {
    g_print("Clicked: %s\n", (gchar *)data);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Menu Structure Demo");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Menu bar
    GtkWidget *menubar = gtk_menu_bar_new();

    // Top-level menu items
    GtkWidget *monchoix = gtk_menu_item_new_with_label("MonChoix");
    GtkWidget *votrechoix = gtk_menu_item_new_with_label("VotreChoix");
    GtkWidget *leurchoix = gtk_menu_item_new_with_label("LeurChoix");

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), monchoix);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), votrechoix);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), leurchoix);

    // Submenu for MonChoix
    GtkWidget *menu_monchoix = gtk_menu_new();
    GtkWidget *sous1 = gtk_menu_item_new_with_label("Sous Choix1");
    GtkWidget *sous2 = gtk_menu_item_new_with_label("Sous Choix2");
    GtkWidget *sous3 = gtk_menu_item_new_with_label("Sous Choix3");

    gtk_menu_shell_append(GTK_MENU_SHELL(menu_monchoix), sous1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_monchoix), sous2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_monchoix), sous3);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(monchoix), menu_monchoix);

    // Horizontal submenu for Sous Choix2
    GtkMenu *horizontal_menu = GTK_MENU(gtk_menu_new());

    GtkWidget *item221 = gtk_menu_item_new_with_label("Choix221");
    GtkWidget *item222 = gtk_menu_item_new_with_label("Choix222");
    GtkWidget *item223 = gtk_menu_item_new_with_label("Choix223");

    g_signal_connect(item221, "activate", G_CALLBACK(on_menu_item_clicked), "Choix221");
    g_signal_connect(item222, "activate", G_CALLBACK(on_menu_item_clicked), "Choix222");
    g_signal_connect(item223, "activate", G_CALLBACK(on_menu_item_clicked), "Choix223");

    gtk_menu_attach(horizontal_menu, item221, 0, 1, 0, 1);
    gtk_menu_attach(horizontal_menu, item222, 1, 2, 0, 1);
    gtk_menu_attach(horizontal_menu, item223, 2, 3, 0, 1);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(sous2), GTK_WIDGET(horizontal_menu));

    // Submenus for VotreChoix and LeurChoix (can be empty or similar)
    GtkWidget *menu_votrechoix = gtk_menu_new();
    GtkWidget *menu_leurchoix = gtk_menu_new();

    GtkWidget *dummy1 = gtk_menu_item_new_with_label("Aide");
    GtkWidget *dummy2 = gtk_menu_item_new_with_label("Info");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_votrechoix), dummy1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_leurchoix), dummy2);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(votrechoix), menu_votrechoix);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(leurchoix), menu_leurchoix);

    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
