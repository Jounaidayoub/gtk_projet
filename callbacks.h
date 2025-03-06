#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>
#include "app_data.h"

// Function to handle drag data received
static void on_drag_data_received(GtkWidget *widget, GdkDragContext *context, gint x, gint y,
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

// Function to generate XML from the Arbre tree structure
void generate_xml_from_arbre(GString *string, Arbre *racine, int indent) {
    if (!racine) return;
    
    // Skip preview_area node which is just a container
    if (strcmp(racine->nom, "preview_area") == 0) {
        // Just process its children
        if (racine->fils) {
            generate_xml_from_arbre(string, racine->fils, indent);
        }
        return;
    }
    
    // Output indentation
    for (int i = 0; i < indent; i++) g_string_append(string, "  ");
    
    // Get widget type - use widget_type field if available, otherwise use node name
    const char *widget_type = racine->widget_type[0] ? racine->widget_type : racine->nom;
    
    // Handle special cases for widget types
    if (strstr(widget_type, "Box:") == widget_type) {
        widget_type = "box";
    } else if (strstr(widget_type, "Label:") == widget_type) {
        widget_type = "label";
    } else if (strstr(widget_type, "Button:") == widget_type) {
        widget_type = "button";
    } else if (strstr(widget_type, "Entry:") == widget_type) {
        widget_type = "entry";
    }
    
    // Output element start
    g_string_append_printf(string, "<%s>\n", widget_type);
    
    // Output widget properties
    for (int i = 0; i < racine->prop_count; i++) {
        for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
        g_string_append_printf(string, "<property name=\"%s\">%s</property>\n", 
                              racine->properties[i].name, racine->properties[i].value);
    }
    
    // Add x,y position if this is a widget in a fixed container
    if (racine->widget && gtk_widget_get_parent(racine->widget) && 
        GTK_IS_FIXED(gtk_widget_get_parent(racine->widget))) {
        int x = 0, y = 0;
        gtk_container_child_get(GTK_CONTAINER(gtk_widget_get_parent(racine->widget)), 
                               racine->widget, "x", &x, "y", &y, NULL);
        
        // Add x position property
        for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
        g_string_append_printf(string, "<property name=\"x\">%d</property>\n", x);
        
        // Add y position property
        for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
        g_string_append_printf(string, "<property name=\"y\">%d</property>\n", y);
    }
    
    // Get size properties
    int width = 0, height = 0;
    if (racine->widget) {
        gtk_widget_get_size_request(racine->widget, &width, &height);
        
        // Only output if size has been explicitly set
        if (width > 0 && height > 0) {
            // Add width property
            for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"width\">%d</property>\n", width);
            
            // Add height property
            for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"height\">%d</property>\n", height);
        }
    }
    
    // Output style properties if available
    for (int i = 0; i < racine->style_prop_count; i++) {
        for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
        
        // Style properties vary by widget type
        const char *style_tag = "style";
        if (strcmp(widget_type, "label") == 0) {
            style_tag = "labelstyle";
        } else if (strcmp(widget_type, "button") == 0) {
            style_tag = "buttonstyle";
        } else if (strcmp(widget_type, "checkbox") == 0) {
            style_tag = "checkboxstyle";
        }
        
        g_string_append_printf(string, "<%s name=\"%s\">%s</%s>\n", 
                              style_tag, racine->style_props[i].name, 
                              racine->style_props[i].value, style_tag);
    }
    
    // Handle children for containers
    if (racine->fils) {
        // If this is a container other than preview_area, wrap children in a children tag
        if (racine->is_container) {
            for (int i = 0; i < indent + 2; i++) g_string_append(string, "  ");
            g_string_append(string, "<child>\n");
            
            generate_xml_from_arbre(string, racine->fils, indent + 4);
            
            for (int i = 0; i < indent + 2; i++) g_string_append(string, "  ");
            g_string_append(string, "</child>\n");
        } else {
            generate_xml_from_arbre(string, racine->fils, indent + 2);
        }
    }
    
    // Process siblings at the same level
    if (racine->frere) {
        // Close current tag before moving to sibling
        for (int i = 0; i < indent; i++) g_string_append(string, "  ");
        g_string_append_printf(string, "</%s>\n", widget_type);
        
        generate_xml_from_arbre(string, racine->frere, indent);
        return; // Skip the closing tag at the end since we already added it
    }
    
    // Close element
    for (int i = 0; i < indent; i++) g_string_append(string, "  ");
    g_string_append_printf(string, "</%s>\n", widget_type);
}

// Export to XML function
void export_to_xml(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    
    // Open a memory stream to write XML
    GString *xml_string = g_string_new("");
    
    // Start immediately with the window tag without XML declaration
    g_string_append(xml_string, "<window>\n");
    
    // Add window properties to match d.html structure
    g_string_append(xml_string, "    <property name=\"title\">GTK UI Builder</property>\n");
    g_string_append(xml_string, "    <property name=\"width\">900</property>\n");
    g_string_append(xml_string, "    <property name=\"height\">700</property>\n");
    g_string_append(xml_string, "    <property name=\"resizable\">1</property>\n");
    g_string_append(xml_string, "    <property name=\"border\">5</property>\n");
    g_string_append(xml_string, "    <property name=\"position\">c</property>\n");
    g_string_append(xml_string, "    <property name=\"x\">400</property>\n");
    g_string_append(xml_string, "    <property name=\"y\">250</property>\n");
    g_string_append(xml_string, "    <property name=\"bgColor\">#ffffff</property>\n");
    g_string_append(xml_string, "    <property name=\"icon\">icon.png</property>\n");
    g_string_append(xml_string, "    <property name=\"headerBar\">1</property>\n");
    g_string_append(xml_string, "    <property name=\"bgImage\">background.png</property>\n");
    g_string_append(xml_string, "    <child>\n\n");
    g_string_append(xml_string, "        <fixed>\n\n");
    g_string_append(xml_string, "            <children>\n");
    
    // Generate XML for all widgets in the tree
    if (app_data->widget_tree) {
        generate_xml_from_arbre(xml_string, app_data->widget_tree, 16);
    }
    
    // Close the XML structure
    g_string_append(xml_string, "            </children>\n");
    g_string_append(xml_string, "        </fixed>\n");
    g_string_append(xml_string, "    </child>\n");
    g_string_append(xml_string, "</window>\n");
    
    // Get the GtkTextBuffer
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app_data->xml_text_view));
    
    // Set the text in the buffer
    gtk_text_buffer_set_text(buffer, xml_string->str, -1);
    
    // Free the GString
    g_string_free(xml_string, TRUE);
    
    g_print("UI exported to XML tab\n");
}

// Run a demo based on the current UI
static void run_demo(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    
    // Placeholder for demo functionality
    g_print("Running demo of the UI (not implemented yet)\n");
}

// Update UI in response to a new container being selected
static void update_ui_for_container(AppData *app_data) {
    GtkWidget *container = app_data->selected_container;
    
    if (!container) {
        // Default case, show widgets for the preview area
        g_print("Showing widgets for preview area\n");
    } else {
        // Show widgets for the selected container
        const gchar *name = gtk_widget_get_name(container);
        g_print("Showing widgets for container: %s\n", name);
    }
}

#endif /* CALLBACKS_H */
