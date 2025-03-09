#include <gtk/gtk.h>

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *treeview;
    GtkTreeStore *store;
    GtkTreeIter iter, child;

    // Create window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Tree View");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Create TreeStore with one column (string type)
    store = gtk_tree_store_new(1, G_TYPE_STRING);

    // Append a parent item
    gtk_tree_store_append(store, &iter, NULL);
    gtk_tree_store_set(store, &iter, 0, "Parent 1", -1);

    // Append a child item under "Parent 1"
    gtk_tree_store_append(store, &child, &iter);
    gtk_tree_store_set(store, &child, 0, "Child 1.1", -1);

    // Append another parent item
    gtk_tree_store_append(store, &iter, NULL);
    gtk_tree_store_set(store, &iter, 0, "Parent 2", -1);

    // Create TreeView and connect it to model
    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

    // Create a column with a renderer
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("Items", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Add tree view to window
    gtk_container_add(GTK_CONTAINER(window), treeview);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.TreeView", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
