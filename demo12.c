#include <gtk/gtk.h>

/* Callback for menu item activation */
static void
on_menu_item_activate(GtkMenuItem *menuitem, gpointer data)
{
    g_print("Activated: %s\n", gtk_menu_item_get_label(menuitem));
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    /* Create the main application window */
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkMenuBar Pack Direction Demo");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Create a vertical box to pack our menu bars */
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    /* --- Menu Bar 1: Default LTR (Left-to-Right) --- */
    GtkWidget *menubar_ltr = gtk_menu_bar_new();

    /* Explicitly set the child pack direction to GTK_PACK_DIRECTION_LTR
       (this is the default, but we show it here for clarity) */
    gtk_menu_bar_set_child_pack_direction(GTK_MENU_BAR(menubar_ltr), GTK_PACK_DIRECTION_LTR);

    /* Create a "File" menu */
    GtkWidget *file_menu = gtk_menu_new();
    GtkWidget *file_item = gtk_menu_item_new_with_label("File");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar_ltr), file_item);

    /* Add submenu items */
    GtkWidget *new_item = gtk_menu_item_new_with_label("New");
    g_signal_connect(new_item, "activate", G_CALLBACK(on_menu_item_activate), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), new_item);

    GtkWidget *open_item = gtk_menu_item_new_with_label("Open");
    g_signal_connect(open_item, "activate", G_CALLBACK(on_menu_item_activate), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_item);

    gtk_box_pack_start(GTK_BOX(vbox), menubar_ltr, FALSE, FALSE, 0);

    /* --- Menu Bar 2: RTL (Right-to-Left) --- */
    GtkWidget *menubar_rtl = gtk_menu_bar_new();

    /* Change the packing direction to GTK_PACK_DIRECTION_RTL */
    gtk_menu_bar_set_child_pack_direction(GTK_MENU_BAR(menubar_rtl), GTK_PACK_DIRECTION_RTL);

    /* Create an "Edit" menu */
    GtkWidget *edit_menu = gtk_menu_new();
    GtkWidget *edit_item = gtk_menu_item_new_with_label("Edit");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_item), edit_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar_rtl), edit_item);

    /* Add submenu items */
    GtkWidget *cut_item = gtk_menu_item_new_with_label("Cut");
    g_signal_connect(cut_item, "activate", G_CALLBACK(on_menu_item_activate), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), cut_item);

    GtkWidget *copy_item = gtk_menu_item_new_with_label("Copy");
    g_signal_connect(copy_item, "activate", G_CALLBACK(on_menu_item_activate), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), copy_item);

    gtk_box_pack_start(GTK_BOX(vbox), menubar_rtl, FALSE, FALSE, 0);

    /* Show all widgets and start the main loop */
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
