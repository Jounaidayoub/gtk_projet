#ifndef DIALOGS_H
#define DIALOGS_H

#include <gtk/gtk.h>
#include "app_data.h"
#include "hierarchy.h"
#include "entry.h"
#include "tree_sync.h"
#include "entry_editing.h" // For entry editing functions
#include "button_editing.h" // Add this include for button editing functions

// Function to show properties dialog
static void show_properties_dialog(GtkWidget *widget, gpointer data)
{
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

// Functions for buttons
static void show_properties_dialog_btn_normal(AppData *app_data)
{
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *name_label, *label_label, *tooltip_label;
    GtkWidget *name_entry, *label_entry, *tooltip_entry;
    GtkWidget *x_label, *y_label, *width_label, *height_label;
    GtkWidget *x_entry, *y_entry, *width_entry, *height_entry;
    GtkWidget *image_path_label, *image_path_entry;
    GtkWidget *has_mnemonic_check;
    gint response;

    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Normal Button Properties",
                                         GTK_WINDOW(app_data->window),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "OK", GTK_RESPONSE_ACCEPT,
                                         "Cancel", GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Create grid for form layout
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // Add container selection at the top of the dialog
    GtkWidget *container_label = gtk_label_new("Add to Container:");
    GtkWidget *container_combo = gtk_combo_box_text_new();
    
    // Add default option (preview area)
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), "Window (Default)");
    
    // Add all containers
    GList *iter;
    for (iter = app_data->containers; iter != NULL; iter = iter->next) {
        GtkWidget *container = GTK_WIDGET(iter->data);
        const gchar *name = gtk_widget_get_name(container);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), name);
    }
    
    // Select currently selected container
    gint active_index = 0;
    if (app_data->selected_container) {
        active_index = g_list_index(app_data->containers, app_data->selected_container) + 1;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(container_combo), active_index);

    // Button identification fields
    name_label = gtk_label_new("Button Name:");
    name_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(name_entry), "button1");
    
    label_label = gtk_label_new("Button Label:");
    label_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(label_entry), "Click Me");
    
    tooltip_label = gtk_label_new("Tooltip Text:");
    tooltip_entry = gtk_entry_new();
    
    // Position fields
    x_label = gtk_label_new("X Position:");
    y_label = gtk_label_new("Y Position:");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), "10");
    gtk_entry_set_text(GTK_ENTRY(y_entry), "10");
    
    // Size fields
    width_label = gtk_label_new("Width:");
    height_label = gtk_label_new("Height:");
    width_entry = gtk_entry_new();
    height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), "120");
    gtk_entry_set_text(GTK_ENTRY(height_entry), "40");

    // Image path field
    image_path_label = gtk_label_new("Image Path (optional):");
    image_path_entry = gtk_entry_new();

    // Mnemonic option
    has_mnemonic_check = gtk_check_button_new_with_label("Has Mnemonic (_X for shortcuts)");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(has_mnemonic_check), TRUE);
    // Style fields
    GtkWidget *police_label = gtk_label_new("Font:");
    GtkWidget *police_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(police_entry), "Sans");

    GtkWidget *color_label = gtk_label_new("Color:");
    GtkWidget *color_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(color_entry), "#000000");

    GtkWidget *taille_label = gtk_label_new("Font Size:");
    GtkWidget *taille_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(taille_entry), "12");

    GtkWidget *gras_check = gtk_check_button_new_with_label("Bold");

    GtkWidget *bgcolor_label = gtk_label_new("Background Color:");
    GtkWidget *bgcolor_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(bgcolor_entry), "#FFFFFF");

    // Add widgets to grid
    int row = 0;
    gtk_grid_attach(GTK_GRID(grid), container_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), container_combo, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), label_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), tooltip_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), tooltip_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), image_path_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), image_path_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), has_mnemonic_check, 0, row, 2, 1);
    row++;

    gtk_grid_attach(GTK_GRID(grid), police_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), police_entry, 1, row, 1, 1);
    row++;

    gtk_grid_attach(GTK_GRID(grid), color_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), color_entry, 1, row, 1, 1);
    row++;

    gtk_grid_attach(GTK_GRID(grid), taille_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), taille_entry, 1, row, 1, 1);
    row++;

    gtk_grid_attach(GTK_GRID(grid), gras_check, 0, row, 2, 1);
    row++;

    gtk_grid_attach(GTK_GRID(grid), bgcolor_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), bgcolor_entry, 1, row, 1, 1);

    // Add grid to dialog
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);
    
    // Run dialog
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (response == GTK_RESPONSE_ACCEPT) {
        // Get values from form
        const gchar *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const gchar *label_text = gtk_entry_get_text(GTK_ENTRY(label_entry));
        const gchar *tooltip = gtk_entry_get_text(GTK_ENTRY(tooltip_entry));
        gint x = atoi(gtk_entry_get_text(GTK_ENTRY(x_entry)));
        gint y = atoi(gtk_entry_get_text(GTK_ENTRY(y_entry)));
        gint width = atoi(gtk_entry_get_text(GTK_ENTRY(width_entry)));
        gint height = atoi(gtk_entry_get_text(GTK_ENTRY(height_entry)));
        const gchar *image_path = gtk_entry_get_text(GTK_ENTRY(image_path_entry));
        gboolean has_mnemonic = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(has_mnemonic_check));
        const gchar *police = gtk_entry_get_text(GTK_ENTRY(police_entry));
        const gchar *color = gtk_entry_get_text(GTK_ENTRY(color_entry));
        gint taille = atoi(gtk_entry_get_text(GTK_ENTRY(taille_entry)));
        gboolean is_gras = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gras_check));
        const gchar *bgcolor = gtk_entry_get_text(GTK_ENTRY(bgcolor_entry));
        // Get the selected container
        gint container_index = gtk_combo_box_get_active(GTK_COMBO_BOX(container_combo));
        GtkWidget *target_container = NULL;
        if (container_index > 0) {
            target_container = g_list_nth_data(app_data->containers, container_index - 1);
        } else {
            target_container = app_data->preview_area;
        }
        
        // Create button image if path is provided
        MonImage *button_image = NULL;
        if (strlen(image_path) > 0) {
            button_image = init_image(image_path, *dim(16, 16), *cord(0, 0));
        }
        
        // Create normal button
        btn *button = btnNormalFixed(
            (gchar*)name,             // Button name
            (gchar*)label_text,       // Button label
            (gchar*)tooltip,          // Tooltip
            target_container,         // Container
            cord(x, y),               // Position
            dim(width, height),       // Dimension
            button_image              // Image (NULL if not provided)
        );
        
        if (button != NULL) {
            // Override mnemonic setting if needed
            button->hasMnemonic = has_mnemonic;
            
            // Create the button widget
            btn *created_button = creer_button(button);
            
            // Apply a default style
            Style *default_style = init_style("Sans", hex_color_init(color), taille, is_gras, bgcolor, 1, 5);
            g_print("back: %s", bgcolor);
            appliquer_style_button(default_style, created_button);
            copy_style_to_btn(created_button, default_style);
            // Add the widget to hierarchy trees
            add_widget_to_both_trees(app_data, created_button->button, "Button", target_container, TRUE, created_button);
            
            // Show all widgets
            gtk_widget_show_all(app_data->preview_area);
        }
    }
    
    gtk_widget_destroy(dialog);
}



static void show_properties_dialog_btn_radio(AppData *app_data){
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid, *gras_check;
    GtkWidget *taille_label, *color_label, *police_label, *labels, *labels_entry;
    GtkWidget *taille_entry, *color_entry, *police_entry;
    GtkWidget *x_label, *y_label, *width_label, *height_label;
    GtkWidget *x_entry, *y_entry, *width_entry, *height_entry;
    GtkWidget *group_label, *group_combo;
    gint response;

    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Radio Button Properties",
                                         GTK_WINDOW(app_data->window),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "OK", GTK_RESPONSE_ACCEPT,
                                         "Cancel", GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Create grid for form layout
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // Add container selection at the top of the dialog
    GtkWidget *container_label = gtk_label_new("Add to Container:");
    GtkWidget *container_combo = gtk_combo_box_text_new();

    // Add default option (preview area)
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), "Window (Default)");

    // Add all containers
    GList *iter;
    for (iter = app_data->containers; iter != NULL; iter = iter->next) {
        GtkWidget *container = GTK_WIDGET(iter->data);
        const gchar *name = gtk_widget_get_name(container);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), name);
    }

    // Select currently selected container
    gint active_index = 0;
    if (app_data->selected_container) {
        active_index = g_list_index(app_data->containers, app_data->selected_container) + 1;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(container_combo), active_index);


    labels = gtk_label_new("Labels:");
    labels_entry = gtk_entry_new();
    // gtk_entry_set_text(GTK_ENTRY(name_entry), "radio1");

    // // Button identification fields
    police_label = gtk_label_new("Buttons police:");
    police_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(police_entry), "Sans");

    color_label = gtk_label_new("Buttons color:");
    color_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(color_entry), "#000");

    taille_label = gtk_label_new("Buttons taille:");
    taille_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(taille_entry), "12");

    gras_check = gtk_check_button_new_with_label("Gras");

    

    // tooltip_label = gtk_label_new("Tooltip Text:");
    // tooltip_entry = gtk_entry_new();

    // Position fields
    x_label = gtk_label_new("X Position:");
    y_label = gtk_label_new("Y Position:");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), "20");
    gtk_entry_set_text(GTK_ENTRY(y_entry), "150");

    // Group selection
    // group_label = gtk_label_new("Group:");
    // group_combo = gtk_combo_box_text_new();
    // gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(group_combo), "None");
    // for (iter = app_data->radio_groups; iter != NULL; iter = iter->next) {
    //     const gchar *group_name = (const gchar *)iter->data;
    //     gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(group_combo), group_name);
    // }
    // gtk_combo_box_set_active(GTK_COMBO_BOX(group_combo), 0);

    // Add widgets to grid
    int row = 0;
    gtk_grid_attach(GTK_GRID(grid), container_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), container_combo, 1, row, 1, 1);
    row++;

    gtk_grid_attach(GTK_GRID(grid), taille_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), taille_entry, 1, row, 1, 1);
    row++;

    gtk_grid_attach(GTK_GRID(grid), color_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), color_entry, 1, row, 1, 1);
    row++;

    gtk_grid_attach(GTK_GRID(grid), police_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), police_entry, 1, row, 1, 1);
    row++;

    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row, 1, 1);
    row++;

    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row, 1, 1);
    row++;
    //
    gtk_grid_attach(GTK_GRID(grid), labels, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), labels_entry, 1, row, 1, 1);
    row++;

    gtk_grid_attach(GTK_GRID(grid), gras_check, 0, row, 2, 1);


    // Add grid to dialog
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    // Run dialog
    response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
        // Get values from form
        const gchar *police_text = gtk_entry_get_text(GTK_ENTRY(police_entry));
        const int taille_int = atoi(gtk_entry_get_text(GTK_ENTRY(taille_entry)));
        const gchar *color_text = gtk_entry_get_text(GTK_ENTRY(color_entry));
        gint x = atoi(gtk_entry_get_text(GTK_ENTRY(x_entry)));
        gint y = atoi(gtk_entry_get_text(GTK_ENTRY(y_entry)));

        gboolean is_gras = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gras_check));

        // gint group_index = gtk_combo_box_get_active(GTK_COMBO_BOX(group_combo));
        // const gchar *group_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(group_combo));
        //
        char *labels_text = gtk_entry_get_text(GTK_ENTRY(labels_entry));
        // Get the selected container
        gint container_index = gtk_combo_box_get_active(GTK_COMBO_BOX(container_combo));
        GtkWidget *target_container = NULL;

        if (container_index > 0) {
            target_container = g_list_nth_data(app_data->containers, container_index - 1);
        } else {
            target_container = app_data->preview_area;
        }

       // gchar *police, HexColor *color, gint taille, gint gras, char *bgcolor, int border, int border_radius
        // creer le style
        Style *default_style = init_style(police_text, hex_color_init(color_text), taille_int, is_gras, NULL, 0, 0);
        // creer le box des boutons
        StyledBox* bx = init_styled_box(0, 1, 8, "", "", "", "", cord(x, y), dim(100, 200), target_container);
        create_styled_box(bx);
        // convertir les labels en un tableau de chaines
        gchar **array = g_strsplit(labels_text, " ", -1); // Split by space
        // creer les radios
        btn** liste = liste_radios(array, default_style, bx);
        gtk_widget_show_all(bx->widget);

            // Show all widgets
            gtk_widget_show_all(app_data->preview_area);

        // Add the box (radioList) to both trees
        add_widget_to_both_trees(app_data, bx->widget, "radioList", bx->container, TRUE, bx);
        //Add the radio buttons to trees
        if (liste) {
            for (int i = 0; liste[i]; i++) {
                add_widget_to_both_trees(app_data, liste[i]->button, "radio", bx->widget, FALSE, liste[i]);
            }
        }
        // }
    }

    gtk_widget_destroy(dialog);
    
    // c
    

};

// static void on_browse_image_clicked(GtkButton *button, gpointer user_data)
// {
//     GtkWidget *entry = GTK_WIDGET(user_data);
//     GtkWidget *dialog;
    
//     dialog = gtk_file_chooser_dialog_new("Open Image",
//                                        NULL,
//                                        GTK_FILE_CHOOSER_ACTION_OPEN,
//                                        "Cancel", GTK_RESPONSE_CANCEL,
//                                        "Open", GTK_RESPONSE_ACCEPT,
//                                        NULL);
    
//     gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 400);

//     // Add filters for image files
//     GtkFileFilter *filter = gtk_file_filter_new();
//     gtk_file_filter_set_name(filter, "Image Files");
//     gtk_file_filter_add_pixbuf_formats(filter);
//     gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
    
//     // Show the dialog and get the result
//     if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
//     {
//         char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
//         gtk_entry_set_text(GTK_ENTRY(entry), filename);
//         g_free(filename);
//     }
    
//     gtk_widget_destroy(dialog);
// }

static void show_image_dialog(AppData *app_data)
{
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *x_label, *y_label, *width_label, *height_label;
    GtkWidget *x_entry, *y_entry, *width_entry, *height_entry;
    GtkWidget *path_label, *path_entry;
    gint response;

    // Création du dialogue
    dialog = gtk_dialog_new_with_buttons("Image Properties",
                                         GTK_WINDOW(app_data->window),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "OK", GTK_RESPONSE_ACCEPT,
                                         "Cancel", GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Création de la grille
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // Sélection du conteneur
    GtkWidget *container_label = gtk_label_new("Add to Container:");
    GtkWidget *container_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), "Window (Default)");

    // Ajout des conteneurs disponibles
    GList *iter;
    for (iter = app_data->containers; iter != NULL; iter = iter->next)
    {
        GtkWidget *container = GTK_WIDGET(iter->data);
        const gchar *name = gtk_widget_get_name(container);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), name);
    }

    // Sélection du conteneur actif
    gint active_index = 0;
    if (app_data->selected_container)
    {
        active_index = g_list_index(app_data->containers, app_data->selected_container) + 1;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(container_combo), active_index);

    // Ajout des widgets à la grille
    gtk_grid_attach(GTK_GRID(grid), container_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), container_combo, 1, 0, 1, 1);

    // Chemin de l'image
    path_label = gtk_label_new("Image Path:");
    path_entry = gtk_entry_new();

    // Create a button for file selection
    GtkWidget *browse_button = gtk_button_new_with_label("Browse...");
    
    GtkWidget *path_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(path_box), path_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(path_box), browse_button, FALSE, FALSE, 0);
    
    g_signal_connect(browse_button, "clicked", G_CALLBACK(on_browse_image_clicked), path_entry);


    // Champs de position
    x_label = gtk_label_new("X Position:");
    y_label = gtk_label_new("Y Position:");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), "10");
    gtk_entry_set_text(GTK_ENTRY(y_entry), "10");

    // Champs de taille
    width_label = gtk_label_new("Width:");
    height_label = gtk_label_new("Height:");
    width_entry = gtk_entry_new();
    height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), "150");
    gtk_entry_set_text(GTK_ENTRY(height_entry), "30");

    // Ajout des widgets à la grille
    gtk_grid_attach(GTK_GRID(grid), path_label,  0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), path_entry,  1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), path_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), path_box, 1, 1, 1, 1);  
    // Use path_box instead of path_entry
    gtk_grid_attach(GTK_GRID(grid), x_label,     0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry,     1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label,     0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry,     1, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), width_label,  0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry,  1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, 5, 1, 1);

    // Ajout de la grille au dialogue
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    // Exécution du dialogue
    response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT)
    {
        // Récupération des valeurs saisies
        const gchar *path1 = gtk_entry_get_text(GTK_ENTRY(path_entry));
        gchar path[300];
        g_strlcpy(path, path1, sizeof(path));

        gint x = atoi(gtk_entry_get_text(GTK_ENTRY(x_entry)));
        gint y = atoi(gtk_entry_get_text(GTK_ENTRY(y_entry)));
        gint width = atoi(gtk_entry_get_text(GTK_ENTRY(width_entry)));
        gint height = atoi(gtk_entry_get_text(GTK_ENTRY(height_entry)));

        // Sélection du conteneur
        gint container_index = gtk_combo_box_get_active(GTK_COMBO_BOX(container_combo));
        GtkWidget *target_container = NULL;

        if (container_index > 0)
        {
            target_container = g_list_nth_data(app_data->containers, container_index - 1);
        }
        else
        {
            target_container = app_data->preview_area;
        }




        // Création de l'image

        MonImage *image = init_image(path, (dimension){height, width}, (coordonnees){x, y});
        creer_image(image);

        gtk_widget_add_events(image->Image, GDK_BUTTON_PRESS_MASK);


        add_widget_to_container_by_type(image->Image,target_container,image->cord.x,image->cord.y,image->dim.width,image->dim.height);

        if (image != NULL)
        {
            g_print("dddddddd");
            GtkWidget *img= creer_image_with_editing(image, app_data);
            gtk_widget_show_all(app_data->preview_area);

        }
    }

    // Destruction du dialogue
    gtk_widget_destroy(dialog);
}





// Function to handle basic entry button click
static void add_image_clicked(GtkWidget *widget, gpointer data)
{
    AppData *app_data = (AppData *)data;
    show_image_dialog(app_data);
}


static void show_properties_dialog_btn_checkbox(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *name_label, *label_label, *tooltip_label;
    GtkWidget *name_entry, *label_entry, *tooltip_entry;
    GtkWidget *x_label, *y_label;
    GtkWidget *x_entry, *y_entry;
    GtkWidget *checked_check;
    gint response;

    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Checkbox Properties",
                                         GTK_WINDOW(app_data->window),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "OK", GTK_RESPONSE_ACCEPT,
                                         "Cancel", GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Create grid for form layout
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // Add container selection
    GtkWidget *container_label = gtk_label_new("Add to Container:");
    GtkWidget *container_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), "Window (Default)");
    
    // Add all containers
    GList *iter;
    for (iter = app_data->containers; iter != NULL; iter = iter->next) {
        GtkWidget *container = GTK_WIDGET(iter->data);
        const gchar *name = gtk_widget_get_name(container);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), name);
    }
    
    // Select currently selected container
    gint active_index = 0;
    if (app_data->selected_container) {
        active_index = g_list_index(app_data->containers, app_data->selected_container) + 1;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(container_combo), active_index);

    // Button identification fields
    name_label = gtk_label_new("Checkbox Name:");
    name_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(name_entry), "checkbox1");
    
    label_label = gtk_label_new("Checkbox Label:");
    label_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(label_entry), "Check me");
    
    tooltip_label = gtk_label_new("Tooltip Text:");
    tooltip_entry = gtk_entry_new();
    
    // Position fields
    x_label = gtk_label_new("X Position:");
    y_label = gtk_label_new("Y Position:");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), "10");
    gtk_entry_set_text(GTK_ENTRY(y_entry), "50");
    
    // Checked state
    checked_check = gtk_check_button_new_with_label("Initially Checked");

    // Add widgets to grid
    int row = 0;
    gtk_grid_attach(GTK_GRID(grid), container_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), container_combo, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), label_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), tooltip_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), tooltip_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), checked_check, 0, row, 2, 1);

    // Add grid to dialog
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);
    
    // Run dialog
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (response == GTK_RESPONSE_ACCEPT) {
        // Get values from form
        const gchar *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const gchar *label_text = gtk_entry_get_text(GTK_ENTRY(label_entry));
        const gchar *tooltip = gtk_entry_get_text(GTK_ENTRY(tooltip_entry));
        gint x = atoi(gtk_entry_get_text(GTK_ENTRY(x_entry)));
        gint y = atoi(gtk_entry_get_text(GTK_ENTRY(y_entry)));
        gboolean is_checked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checked_check));
        
        // Get the selected container
        gint container_index = gtk_combo_box_get_active(GTK_COMBO_BOX(container_combo));
        GtkWidget *target_container = NULL;
        
        if (container_index > 0) {
            target_container = g_list_nth_data(app_data->containers, container_index - 1);
        } else {
            target_container = app_data->preview_area;
        }


        btn *checkbox_button = NULL;
        if(GTK_IS_FIXED(target_container)){
        // Create checkbox button
        checkbox_button = btnCheckFixed(
            (gchar*)name,             // Button name
            (gchar*)label_text,       // Button label
            (gchar*)tooltip,          // Tooltip
            cord(x, y),               // Position
            target_container,         // Container
            is_checked,               // Checked state
            NULL                      // Image (NULL for checkbox)
        );
            }
        else{
            checkbox_button = btnCheck(
                (gchar*)name,             // Button name
                (gchar*)label_text,       // Button label
                (gchar*)tooltip,          // Tooltip
                NULL,               // Position
                target_container,         // Container
                is_checked,               // Checked state
                NULL                      // Image (NULL for checkbox)
            );
       }

        
        if (checkbox_button != NULL) {
            // Create the button widget with editing capabilities
            GtkWidget *created_button = create_checkbox_button_with_editing(checkbox_button, app_data);
            
            // Show all widgets
            gtk_widget_show_all(app_data->preview_area);
        }
    }
    
    gtk_widget_destroy(dialog);
}

static void show_properties_dialog_btn_toggle(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *name_label, *label_label, *tooltip_label;
    GtkWidget *name_entry, *label_entry, *tooltip_entry;
    GtkWidget *x_label, *y_label, *width_label, *height_label;
    GtkWidget *x_entry, *y_entry, *width_entry, *height_entry;
    GtkWidget *toggled_check;
    gint response;

    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Toggle Button Properties",
                                         GTK_WINDOW(app_data->window),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "OK", GTK_RESPONSE_ACCEPT,
                                         "Cancel", GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Create grid for form layout
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // Add container selection
    GtkWidget *container_label = gtk_label_new("Add to Container:");
    GtkWidget *container_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), "Window (Default)");
    
    // Add all containers
    GList *iter;
    for (iter = app_data->containers; iter != NULL; iter = iter->next) {
        GtkWidget *container = GTK_WIDGET(iter->data);
        const gchar *name = gtk_widget_get_name(container);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), name);
    }
    
    // Select currently selected container
    gint active_index = 0;
    if (app_data->selected_container) {
        active_index = g_list_index(app_data->containers, app_data->selected_container) + 1;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(container_combo), active_index);

    // Button identification fields
    name_label = gtk_label_new("Button Name:");
    name_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(name_entry), "toggle1");
    
    label_label = gtk_label_new("Button Label:");
    label_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(label_entry), "Toggle Me");
    
    tooltip_label = gtk_label_new("Tooltip Text:");
    tooltip_entry = gtk_entry_new();
    
    // Position fields
    x_label = gtk_label_new("X Position:");
    y_label = gtk_label_new("Y Position:");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), "10");
    gtk_entry_set_text(GTK_ENTRY(y_entry), "100");
    
    // Size fields
    width_label = gtk_label_new("Width:");
    height_label = gtk_label_new("Height:");
    width_entry = gtk_entry_new();
    height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), "120");
    gtk_entry_set_text(GTK_ENTRY(height_entry), "40");
    
    // Toggled state
    toggled_check = gtk_check_button_new_with_label("Initially Toggled");

    // Add widgets to grid
    int row = 0;
    gtk_grid_attach(GTK_GRID(grid), container_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), container_combo, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), label_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), tooltip_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), tooltip_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), toggled_check, 0, row, 2, 1);

    // Add grid to dialog
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);
    
    // Run dialog
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (response == GTK_RESPONSE_ACCEPT) {
        // Get values from form
        const gchar *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const gchar *label_text = gtk_entry_get_text(GTK_ENTRY(label_entry));
        const gchar *tooltip = gtk_entry_get_text(GTK_ENTRY(tooltip_entry));
        gint x = atoi(gtk_entry_get_text(GTK_ENTRY(x_entry)));
        gint y = atoi(gtk_entry_get_text(GTK_ENTRY(y_entry)));
        gint width = atoi(gtk_entry_get_text(GTK_ENTRY(width_entry)));
        gint height = atoi(gtk_entry_get_text(GTK_ENTRY(height_entry)));
        gboolean is_toggled = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggled_check));
        
        // Get the selected container
        gint container_index = gtk_combo_box_get_active(GTK_COMBO_BOX(container_combo));
        GtkWidget *target_container = NULL;
        
        if (container_index > 0) {
            target_container = g_list_nth_data(app_data->containers, container_index - 1);
        } else {
            target_container = app_data->preview_area;
        }
        
        // Create toggle button
        btn *toggle_button = btnToggleFixed(
            (gchar*)name,             // Button name
            (gchar*)label_text,       // Button label
            (gchar*)tooltip,          // Tooltip
            target_container,         // Container
            cord(x, y),               // Position
            dim(width, height),       // Dimension
            NULL                      // Image (NULL for toggle button)
        );
        
        if (toggle_button != NULL) {
            // Set initial toggle state
            toggle_button->isChecked = is_toggled;
            
            // Create the button widget with editing capabilities
            GtkWidget *created_button = create_toggle_button_with_editing(toggle_button, app_data);
            
            // Show all widgets
            gtk_widget_show_all(app_data->preview_area);
        }
    }
    
    gtk_widget_destroy(dialog);
}

static void show_properties_dialog_btn_spin(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *name_label, *tooltip_label;
    GtkWidget *name_entry, *tooltip_entry;
    GtkWidget *x_label, *y_label;
    GtkWidget *x_entry, *y_entry;
    GtkWidget *min_label, *max_label, *step_label, *digits_label, *value_label;
    GtkWidget *min_entry, *max_entry, *step_entry, *digits_entry, *value_entry;
    gint response;

    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Spin Button Properties",
                                         GTK_WINDOW(app_data->window),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "OK", GTK_RESPONSE_ACCEPT,
                                         "Cancel", GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Create grid for form layout
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // Add container selection
    GtkWidget *container_label = gtk_label_new("Add to Container:");
    GtkWidget *container_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), "Window (Default)");
    
    // Add all containers
    GList *iter;
    for (iter = app_data->containers; iter != NULL; iter = iter->next) {
        GtkWidget *container = GTK_WIDGET(iter->data);
        const gchar *name = gtk_widget_get_name(container);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), name);
    }
    
    // Select currently selected container
    gint active_index = 0;
    if (app_data->selected_container) {
        active_index = g_list_index(app_data->containers, app_data->selected_container) + 1;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(container_combo), active_index);

    // Button identification fields
    name_label = gtk_label_new("Spin Button Name:");
    name_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(name_entry), "spin1");
    
    tooltip_label = gtk_label_new("Tooltip Text:");
    tooltip_entry = gtk_entry_new();
    
    // Position fields
    x_label = gtk_label_new("X Position:");
    y_label = gtk_label_new("Y Position:");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), "10");
    gtk_entry_set_text(GTK_ENTRY(y_entry), "150");
    
    // Spin button properties
    min_label = gtk_label_new("Minimum Value:");
    max_label = gtk_label_new("Maximum Value:");
    step_label = gtk_label_new("Step Increment:");
    digits_label = gtk_label_new("Digits (Decimals):");
    value_label = gtk_label_new("Initial Value:");
    
    min_entry = gtk_entry_new();
    max_entry = gtk_entry_new();
    step_entry = gtk_entry_new();
    digits_entry = gtk_entry_new();
    value_entry = gtk_entry_new();
    
    gtk_entry_set_text(GTK_ENTRY(min_entry), "0");
    gtk_entry_set_text(GTK_ENTRY(max_entry), "100");
    gtk_entry_set_text(GTK_ENTRY(step_entry), "1");
    gtk_entry_set_text(GTK_ENTRY(digits_entry), "0");
    gtk_entry_set_text(GTK_ENTRY(value_entry), "0");

    // Add widgets to grid
    int row = 0;
    gtk_grid_attach(GTK_GRID(grid), container_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), container_combo, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), tooltip_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), tooltip_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), min_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), min_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), max_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), max_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), step_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), step_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), digits_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), digits_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), value_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), value_entry, 1, row, 1, 1);

    // Add grid to dialog
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);
    
    // Run dialog
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (response == GTK_RESPONSE_ACCEPT) {
        for (int i = 0; i < 10; i++) {
            g_print("asdhvasjbdahsdvadj");
        }
        // Get values from form
        const gchar *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const gchar *tooltip = gtk_entry_get_text(GTK_ENTRY(tooltip_entry));
        gint x = atoi(gtk_entry_get_text(GTK_ENTRY(x_entry)));
        gint y = atoi(gtk_entry_get_text(GTK_ENTRY(y_entry)));
        gdouble min_value = g_strtod(gtk_entry_get_text(GTK_ENTRY(min_entry)), NULL);
        gdouble max_value = g_strtod(gtk_entry_get_text(GTK_ENTRY(max_entry)), NULL);
        gdouble step_value = g_strtod(gtk_entry_get_text(GTK_ENTRY(step_entry)), NULL);
        guint digits = atoi(gtk_entry_get_text(GTK_ENTRY(digits_entry)));
        gdouble initial_value = g_strtod(gtk_entry_get_text(GTK_ENTRY(value_entry)), NULL);
        
        // Get the selected container
        gint container_index = gtk_combo_box_get_active(GTK_COMBO_BOX(container_combo));
        GtkWidget *target_container = NULL;
        
        if (container_index > 0) {
            target_container = g_list_nth_data(app_data->containers, container_index - 1);
        } else {
            target_container = app_data->preview_area;
        }
        // print the targed container for debug
        g_print("Container: %s\n", gtk_widget_get_name(target_container));
        
        // Create spin object
        spinObj *sp = (spinObj*)malloc(sizeof(spinObj));
        if (sp) {
            
            
            sp->borneInf = min_value;
            sp->borneSup = max_value;
            sp->step = step_value;
            sp->digits = digits;
            sp->start = initial_value;
            
            // Create spin button
            //print then selected container for debug
            // g_print("Container: jhasvfajsdاتشريلاشتسيارتشساي %s\n", gtk_widget_get_name(target_container));
            // g_print("");
            btn *spin_button = NULL;
            if (GTK_IS_FIXED(target_container)) {
                g_print("Fixed container\n");
                spin_button = btnSpinFixed(
                    (gchar*)name,             // Button name
                    "",                       // Label (not used for spin button)
                    (gchar*)tooltip,          // Tooltip
                    dim(80, 30),              // Dimension
                    cord(x, y),               // Position
                    target_container,         // Container
                    sp                        // Spin object
                );
            }else {
                spin_button = btnSpin(
                    (gchar*)name,             // Button name
                    "",                       // Label (not used for spin button)
                    (gchar*)tooltip,          // Tooltip
                    // dim(80, 30),              // Dimension
                    // cord(x, y),               // Position
                    NULL,
                    target_container,         // Container
                    sp                        // Spin object
                );
            }
            // btn *spin_button = btnSpinFixed(
            //     (gchar*)name,             // Button name
            //     "",                       // Label (not used for spin button)
            //     (gchar*)tooltip,          // Tooltip
            //     dim(80, 30),              // Dimension
            //     cord(x, y),               // Position
            //     target_container,         // Container
            //     sp                        // Spin object
            // );
            
            if (spin_button != NULL) {
                // Create the button widget with editing capabilities
                GtkWidget *created_button = create_spin_button_with_editing(spin_button, app_data);
                
                // Show all widgets
                gtk_widget_show_all(app_data->preview_area);
            }
        }
    }
    
    gtk_widget_destroy(dialog);
}

static void show_properties_dialog_btn_switch(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    
    GtkWidget *name_label, *tooltip_label;
    GtkWidget *name_entry, *tooltip_entry;
    GtkWidget *x_label, *y_label;
    GtkWidget *x_entry, *y_entry;
    GtkWidget *active_check;
    gint response;

    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Switch Properties",
                                         GTK_WINDOW(app_data->window),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "OK", GTK_RESPONSE_ACCEPT,
                                         "Cancel", GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Create grid for form layout
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // Add container selection
    GtkWidget *container_label = gtk_label_new("Add to Container:");
    GtkWidget *container_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), "Window (Default)");
    
    // Add all containers
    GList *iter;
    for (iter = app_data->containers; iter != NULL; iter = iter->next) {
        GtkWidget *container = GTK_WIDGET(iter->data);
        const gchar *name = gtk_widget_get_name(container);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), name);
    }
    
    // Select currently selected container
    gint active_index = 0;
    if (app_data->selected_container) {
        active_index = g_list_index(app_data->containers, app_data->selected_container) + 1;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(container_combo), active_index);

    // Switch identification fields
    name_label = gtk_label_new("Switch Name:");
    name_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(name_entry), "switch1");
    
    tooltip_label = gtk_label_new("Tooltip Text:");
    tooltip_entry = gtk_entry_new();
    
    // Position fields
    x_label = gtk_label_new("X Position:");
    y_label = gtk_label_new("Y Position:");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), "10");
    gtk_entry_set_text(GTK_ENTRY(y_entry), "200");
    
    // Active state
    active_check = gtk_check_button_new_with_label("Initially Active");

    // Add widgets to grid
    int row = 0;
    gtk_grid_attach(GTK_GRID(grid), container_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), container_combo, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), tooltip_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), tooltip_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, row, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, row, 1, 1);
    row++;
    
    gtk_grid_attach(GTK_GRID(grid), active_check, 0, row, 2, 1);

    // Add grid to dialog
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);
    
    // Run dialog
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (response == GTK_RESPONSE_ACCEPT) {
        // Get values from form
        const gchar *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        const gchar *tooltip = gtk_entry_get_text(GTK_ENTRY(tooltip_entry));
        gint x = atoi(gtk_entry_get_text(GTK_ENTRY(x_entry)));
        gint y = atoi(gtk_entry_get_text(GTK_ENTRY(y_entry)));
        gboolean is_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(active_check));
        
        // Get the selected container
        gint container_index = gtk_combo_box_get_active(GTK_COMBO_BOX(container_combo));
        GtkWidget *target_container = NULL;
        
        if (container_index > 0) {
            target_container = g_list_nth_data(app_data->containers, container_index - 1);
        } else {
            target_container = app_data->preview_area;
        }
        
        // Create switch button
        btn *switch_button = btnSwitchFixed(
            (gchar*)name,             // Button name
            "",
            (gchar*)tooltip,          // Tooltip
            target_container,         // Container
            cord(x, y),               // Position
            dim(80, 30),              // Dimension
            NULL,                     // Image (NULL for switch button)
            is_active                 // Active state
        );
        
        if (switch_button != NULL) {
            // Create the button widget with editing capabilities
            GtkWidget *created_button = create_switch_button_with_editing(switch_button, app_data);
            
            // Show all widgets
            gtk_widget_show_all(app_data->preview_area);
        }
    }
    
    gtk_widget_destroy(dialog);
}

// static void show_properties_dialog_btn_checkbox(GtkWidget *widget, gpointer data) {};
// static void show_properties_dialog_btn_toggle(GtkWidget *widget, gpointer data) {};
// static void show_properties_dialog_btn_spin(GtkWidget *widget, gpointer data) {};
// static void show_properties_dialog_btn_switch(GtkWidget *widget, gpointer data) {};

// Function to show dialog for basic entry configuration
static void show_basic_entry_dialog(AppData *app_data)
{
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *x_label, *y_label, *width_label, *height_label;
    GtkWidget *x_entry, *y_entry, *width_entry, *height_entry;
    GtkWidget *editable_check, *visible_check;
    GtkWidget *placeholder_label, *max_len_label, *default_text_label;
    GtkWidget *placeholder_entry, *max_len_entry, *default_text_entry;
    gint response;

    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Basic Entry Properties",
                                         GTK_WINDOW(app_data->window),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "OK", GTK_RESPONSE_ACCEPT,
                                         "Cancel", GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Create grid for form layout
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // Add container selection at the top of the dialog
    GtkWidget *container_label = gtk_label_new("Add to Container:");
    GtkWidget *container_combo = gtk_combo_box_text_new();

    // Add default option (preview area)
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), "Window (Default)");

    // Add all containers
    GList *iter;
    for (iter = app_data->containers; iter != NULL; iter = iter->next)
    {
        GtkWidget *container = GTK_WIDGET(iter->data);
        const gchar *name = gtk_widget_get_name(container);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), name);
    }

    // Select currently selected container
    gint active_index = 0;
    if (app_data->selected_container)
    {
        active_index = g_list_index(app_data->containers, app_data->selected_container) + 1;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(container_combo), active_index);

    // Add container selection to the top of the form
    gtk_grid_attach(GTK_GRID(grid), container_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), container_combo, 1, 0, 1, 1);

    // Position fields
    x_label = gtk_label_new("X Position:");
    y_label = gtk_label_new("Y Position:");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), "10");
    gtk_entry_set_text(GTK_ENTRY(y_entry), "10");

    // Size fields
    width_label = gtk_label_new("Width:");
    height_label = gtk_label_new("Height:");
    width_entry = gtk_entry_new();
    height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), "150");
    gtk_entry_set_text(GTK_ENTRY(height_entry), "30");

    // Checkbox fields
    editable_check = gtk_check_button_new_with_label("Editable");
    visible_check = gtk_check_button_new_with_label("Visible");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(editable_check), TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(visible_check), TRUE);

    // Text fields
    placeholder_label = gtk_label_new("Placeholder Text:");
    max_len_label = gtk_label_new("Max Length:");
    default_text_label = gtk_label_new("Default Text:");
    placeholder_entry = gtk_entry_new();
    max_len_entry = gtk_entry_new();
    default_text_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(placeholder_entry), "Enter text here...");
    gtk_entry_set_text(GTK_ENTRY(max_len_entry), "0");

    // Add widgets to grid
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), width_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), editable_check, 0, 5, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), visible_check, 0, 6, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), placeholder_label, 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), placeholder_entry, 1, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), max_len_label, 0, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), max_len_entry, 1, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), default_text_label, 0, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), default_text_entry, 1, 9, 1, 1);

    // Add grid to dialog
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    // Run dialog
    response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT)
    {
        // Get values from form
        gint x = atoi(gtk_entry_get_text(GTK_ENTRY(x_entry)));
        gint y = atoi(gtk_entry_get_text(GTK_ENTRY(y_entry)));
        gint width = atoi(gtk_entry_get_text(GTK_ENTRY(width_entry)));
        gint height = atoi(gtk_entry_get_text(GTK_ENTRY(height_entry)));
        gboolean is_editable = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(editable_check));
        gboolean is_visible = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(visible_check));
        const gchar *placeholder = gtk_entry_get_text(GTK_ENTRY(placeholder_entry));
        gint max_len = atoi(gtk_entry_get_text(GTK_ENTRY(max_len_entry)));
        const gchar *default_text = gtk_entry_get_text(GTK_ENTRY(default_text_entry));

        // Get the selected container
        gint container_index = gtk_combo_box_get_active(GTK_COMBO_BOX(container_combo));
        GtkWidget *target_container = NULL;

        if (container_index > 0)
        {
            target_container = g_list_nth_data(app_data->containers, container_index - 1);
        }
        else
        {
            target_container = app_data->preview_area;
        }

        // Create basic entry with the selected container
        entry_type_basic *entry_basic = Init_Entry_Basic(
            dim(width, height),
            is_editable,
            is_visible,
            placeholder,
            max_len,
            default_text,
            target_container,
            cord(x, y));

        if (entry_basic != NULL)
        {
            // Use the editing-enabled widget creation function
            GtkWidget *entry_widget = creer_entry_basic_with_editing(entry_basic, app_data);

            // No need to call add_widget_to_both_trees here since it's done in creer_entry_basic_with_editing

            // Show all widgets
            gtk_widget_show_all(app_data->preview_area);
        }
    }

    gtk_widget_destroy(dialog);
}

// Function to show dialog for password entry configuration
static void show_password_entry_dialog(AppData *app_data)
{
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *x_label, *y_label, *width_label, *height_label;
    GtkWidget *x_entry, *y_entry, *width_entry, *height_entry;
    GtkWidget *placeholder_label, *invisible_char_label;
    GtkWidget *placeholder_entry, *invisible_char_entry;
    gint response;

    // Create dialog
    dialog = gtk_dialog_new_with_buttons("Password Entry Properties",
                                         GTK_WINDOW(app_data->window),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "OK", GTK_RESPONSE_ACCEPT,
                                         "Cancel", GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Create grid for form layout
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    // Add container selection at the top of the dialog
    GtkWidget *container_label = gtk_label_new("Add to Container:");
    GtkWidget *container_combo = gtk_combo_box_text_new();

    // Add default option (preview area)
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), "Window (Default)");

    // Add all containers
    GList *iter;
    for (iter = app_data->containers; iter != NULL; iter = iter->next)
    {
        GtkWidget *container = GTK_WIDGET(iter->data);
        const gchar *name = gtk_widget_get_name(container);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(container_combo), name);
    }

    // Select currently selected container
    gint active_index = 0;
    if (app_data->selected_container)
    {
        active_index = g_list_index(app_data->containers, app_data->selected_container) + 1;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(container_combo), active_index);

    // Add container selection to the top of the form
    gtk_grid_attach(GTK_GRID(grid), container_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), container_combo, 1, 0, 1, 1);

    // Position fields
    x_label = gtk_label_new("X Position:");
    y_label = gtk_label_new("Y Position:");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(x_entry), "10");
    gtk_entry_set_text(GTK_ENTRY(y_entry), "50"); // Different default Y from basic entry

    // Size fields
    width_label = gtk_label_new("Width:");
    height_label = gtk_label_new("Height:");
    width_entry = gtk_entry_new();
    height_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(width_entry), "150");
    gtk_entry_set_text(GTK_ENTRY(height_entry), "30");

    // Password-specific fields
    placeholder_label = gtk_label_new("Placeholder Text:");
    invisible_char_label = gtk_label_new("Mask Character:");
    placeholder_entry = gtk_entry_new();
    invisible_char_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(placeholder_entry), "Enter password...");
    gtk_entry_set_text(GTK_ENTRY(invisible_char_entry), "*");
    gtk_entry_set_max_length(GTK_ENTRY(invisible_char_entry), 1); // Only one character

    // Add widgets to grid
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), width_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), placeholder_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), placeholder_entry, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), invisible_char_label, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), invisible_char_entry, 1, 6, 1, 1);

    // Add grid to dialog
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    // Run dialog
    response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT)
    {
        // Get values from form
        gint x = atoi(gtk_entry_get_text(GTK_ENTRY(x_entry)));
        gint y = atoi(gtk_entry_get_text(GTK_ENTRY(y_entry)));
        gint width = atoi(gtk_entry_get_text(GTK_ENTRY(width_entry)));
        gint height = atoi(gtk_entry_get_text(GTK_ENTRY(height_entry)));
        const gchar *placeholder = gtk_entry_get_text(GTK_ENTRY(placeholder_entry));
        const gchar *invisible_char_text = gtk_entry_get_text(GTK_ENTRY(invisible_char_entry));

        // Default to '*' if no character is provided
        gchar invisible_char = (invisible_char_text && invisible_char_text[0]) ? invisible_char_text[0] : '*';

        // Get the selected container
        gint container_index = gtk_combo_box_get_active(GTK_COMBO_BOX(container_combo));
        GtkWidget *target_container = NULL;

        if (container_index > 0)
        {
            target_container = g_list_nth_data(app_data->containers, container_index - 1);
        }
        else
        {
            target_container = app_data->preview_area;
        }

        // Create password entry
        entry_type_password *entry_password = Init_Entry_Password(
            dim(width, height), // Using the dimension macro
            placeholder,
            invisible_char,
            target_container,
            cord(x, y) // Using the coordinates macro
        );

        if (entry_password != NULL)
        {
            // Use the editing-enabled widget creation function
            GtkWidget *entry_widget = creer_entry_pass_with_editing(entry_password, app_data);

            // No need to call add_widget_to_both_trees here since it's done in creer_entry_pass_with_editing

            // Show all widgets
            gtk_widget_show_all(app_data->preview_area);
        }
    }

    gtk_widget_destroy(dialog);
}
static void add_btn_normal_clicked(GtkWidget *widget, gpointer data)
{
    AppData *app_data = (AppData *)data;
    show_properties_dialog_btn_normal(app_data);
}

static void add_btn_radio_clicked(GtkWidget *widget, gpointer data)
{
    AppData *app_data = (AppData *)data;
    show_properties_dialog_btn_radio(app_data);
}

// Function to handle basic entry button click
static void add_basic_entry_clicked(GtkWidget *widget, gpointer data)
{
    AppData *app_data = (AppData *)data;
    show_basic_entry_dialog(app_data);
}

// Function to handle password entry button click
static void add_password_entry_clicked(GtkWidget *widget, gpointer data)
{
    AppData *app_data = (AppData *)data;
    show_password_entry_dialog(app_data);
}

#endif /* DIALOGS_H */