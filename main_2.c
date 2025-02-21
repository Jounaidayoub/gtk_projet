#include <gtk/gtk.h>
#include "global.h"
// #include "Scale.h"
// #include "scroll.h"

// Function to show properties dialog
void show_properties_dialog(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *label;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;

    dialog = gtk_dialog_new_with_buttons("Widget Properties",
                                         GTK_WINDOW(data),
                                         flags,
                                         "_OK",
                                         GTK_RESPONSE_OK,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    label = gtk_label_new("Enter properties for the widget:");
    gtk_container_add(GTK_CONTAINER(content_area), label);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Function to handle drag data received
void on_drag_data_received(GtkWidget *widget, GdkDragContext *context, gint x, gint y,
                           GtkSelectionData *data, guint info, guint time, gpointer user_data) {
    const gchar *widget_type = (const gchar *)gtk_selection_data_get_data(data);
    GtkWidget *new_widget = NULL;

    if (g_strcmp0(widget_type, "Label") == 0) {
        new_widget = gtk_label_new("New Label");
    } else if (g_strcmp0(widget_type, "Button") == 0) {
        new_widget = gtk_button_new_with_label("New Button");
    } else if (g_strcmp0(widget_type, "Entry") == 0) {
        new_widget = gtk_entry_new();
    } else if (g_strcmp0(widget_type, "ComboBox") == 0) {
        new_widget = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(new_widget), "Option 1");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(new_widget), "Option 2");
    } else if (g_strcmp0(widget_type, "Scale") == 0) {
        new_widget = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    }

    if (new_widget) {
        gtk_grid_attach(GTK_GRID(widget), new_widget, x / 100, y / 50, 1, 1);
        gtk_widget_show_all(widget);
    }

    gtk_drag_finish(context, TRUE, FALSE, time);
}

// Function to export layout to XML
void export_to_xml(GtkWidget *widget, gpointer data) {
    // Implement XML export logic here
    g_print("Export to XML\n");
}

// Function to run demo
void run_demo(GtkWidget *widget, gpointer data) {
    // Implement demo run logic here
    g_print("Run Demo\n");
}

// Function to draw a window inside the preview area
void draw_window(GtkWidget *widget, gpointer data) {
    GtkWidget *preview_grid = GTK_WIDGET(data);
    GtkWidget *window_frame = gtk_frame_new("Window");
    gtk_widget_set_size_request(window_frame, 200, 150);
    gtk_grid_attach(GTK_GRID(preview_grid), window_frame, 0, 0, 1, 1);
    gtk_widget_show_all(preview_grid);
}

int main(int argc, char *argv[]) {
    // Initialisation de GTK
    gtk_init(&argc, &argv);

    // Dimensions et coordonnées de la fenêtre
    dimension dim = {400, 900}; // Largeur: 800px, Hauteur: 600px
    coordonnees cord = {400, 250}; // Position: x=200, y=100

    // Couleur de fond
    HexColor *bg_color = hex_color_init("#ffffff"); // Couleur rose

    // Image de fond
    MonImage bg_img = {
           .path ="ex_img.jpg" , // Remplacez par le chemin de votre image
             .dim = dim,
           .cord = cord
    };

    // Initialisation de la fenêtre
    Mywindow *maFenetre = init_window("                                             Presentation Ordinateur", "icon1.png",
            dim,                       // Dimensions
            0,                        // Bordure
            FALSE,                      // Redimensionnable
            GTK_WIN_POS_CENTER,        // Position: Centrée
            cord,                      // Coordonnées
            *bg_color,                 // Couleur de fond
            bg_img                     // Image de fond
    );

    // Création de la fenêtre
    create_window(maFenetre);

    // Create main horizontal box to hold side panel, content, and right panel
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(maFenetre->window), main_box);

    // Create side panel (left vertical box)
    GtkWidget *side_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(side_panel, 200, -1);  // Fixed width for side panel

    // Top section: Container buttons
    GtkWidget *container_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(container_box), gtk_label_new("Containers"), FALSE, FALSE, 5);

    // Add container buttons
    const char *container_labels[] = {"Fixed", "Box", "Frame", "ScrolledWindow"};
    for (int i = 0; i < 4; i++) {
        btn* container_btn = creer_button(btnNormalFixed(container_labels[i], container_labels[i], 
            "Add container", side_panel, cord(5, 30*i), dim(190, 25), NULL));
        gtk_box_pack_start(GTK_BOX(container_box), container_btn->button, FALSE, FALSE, 0);
    }
    gtk_box_pack_start(GTK_BOX(side_panel), container_box, FALSE, FALSE, 5);

    // Middle section: Widget buttons
    GtkWidget *widget_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(widget_box), gtk_label_new("Widgets"), FALSE, FALSE, 5);

    // Add widget buttons
    const char *widget_labels[] = {"Label", "Button", "Entry", "ComboBox", "Scale"};
    for (int i = 0; i < 5; i++) {
        btn* widget_btn = creer_button(btnNormalFixed(widget_labels[i], widget_labels[i],
            "Add widget", side_panel, cord(5, 30*i), dim(190, 25), NULL));
        g_signal_connect(widget_btn->button, "clicked", G_CALLBACK(show_properties_dialog), maFenetre->window);
        gtk_box_pack_start(GTK_BOX(widget_box), widget_btn->button, FALSE, FALSE, 0);

        // Enable drag and drop for widget buttons
        gtk_drag_source_set(widget_btn->button, GDK_BUTTON1_MASK, NULL, 0, GDK_ACTION_COPY);
        gtk_drag_source_add_text_targets(widget_btn->button);
        g_signal_connect(widget_btn->button, "drag-data-get", G_CALLBACK(gtk_selection_data_set_text), (gpointer)widget_labels[i]);
    }
    gtk_box_pack_start(GTK_BOX(side_panel), widget_box, FALSE, FALSE, 5);

    // Bottom section: Tree view
    GtkWidget *tree_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(tree_scroll),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

    GtkTreeStore *tree_store = gtk_tree_store_new(1, G_TYPE_STRING);
    GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(tree_store));

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(
        "Widget Hierarchy", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    // Add root items
    GtkTreeIter iter;
    gtk_tree_store_append(tree_store, &iter, NULL);
    gtk_tree_store_set(tree_store, &iter, 0, "Main Window", -1);

    gtk_container_add(GTK_CONTAINER(tree_scroll), tree_view);
    gtk_box_pack_start(GTK_BOX(side_panel), tree_scroll, TRUE, TRUE, 5);

    // Add side panel to main box
    gtk_box_pack_start(GTK_BOX(main_box), side_panel, FALSE, FALSE, 0);

    // Create fixed container for main content
    fixedo *fixed = init_fixed("principal", dim);
    creer_fixed(fixed);

    // Create preview area with frame and grid
    GtkWidget *preview_frame = gtk_frame_new("Preview Area");
    gtk_widget_set_size_request(preview_frame, 400, 600);
    GtkWidget *preview_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(preview_frame), preview_grid);
    fixed_add_widget(fixed, preview_frame, 200, 0);

    // Enable drag and drop for preview grid
    gtk_drag_dest_set(preview_grid, GTK_DEST_DEFAULT_ALL, NULL, 0, GDK_ACTION_COPY);
    gtk_drag_dest_add_text_targets(preview_grid);
    g_signal_connect(preview_grid, "drag-data-received", G_CALLBACK(on_drag_data_received), NULL);

    // Add fixed container to main box
    gtk_box_pack_start(GTK_BOX(main_box), fixed->fixed_container, TRUE, TRUE, 0);

    // Create right panel for properties preview
    GtkWidget *right_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(right_panel, 200, -1);  // Fixed width for right panel
    gtk_box_pack_start(GTK_BOX(right_panel), gtk_label_new("Properties"), FALSE, FALSE, 5);

    // Add right panel to main box
    gtk_box_pack_start(GTK_BOX(main_box), right_panel, FALSE, FALSE, 0);

    // Bottom right buttons container
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *export_button = gtk_button_new_with_label("Export XML");
    GtkWidget *run_button = gtk_button_new_with_label("Run Demo");
    GtkWidget *exit_button = gtk_button_new_with_label("Exit");

    g_signal_connect(export_button, "clicked", G_CALLBACK(export_to_xml), NULL);
    g_signal_connect(run_button, "clicked", G_CALLBACK(run_demo), NULL);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_box_pack_start(GTK_BOX(button_box), export_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), run_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), exit_button, FALSE, FALSE, 0);

    gtk_box_pack_end(GTK_BOX(main_box), button_box, FALSE, FALSE, 5);

    // Add button to draw a window inside the preview area
    GtkWidget *draw_window_button = gtk_button_new_with_label("Draw Window");
    g_signal_connect(draw_window_button, "clicked", G_CALLBACK(draw_window), preview_grid);
    gtk_box_pack_start(GTK_BOX(right_panel), draw_window_button, FALSE, FALSE, 5);

    // Show all widgets
    gtk_widget_show_all(maFenetre->window);
    gtk_main();

    return 0;
}