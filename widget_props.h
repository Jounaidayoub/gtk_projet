#ifndef WIDGET_PROPS_H
#define WIDGET_PROPS_H

#include <gtk/gtk.h>
#include "containers_list.h"

// Helper function to populate widget properties in the Arbre node
void populate_widget_properties(Arbre *node, GtkWidget *widget) {
    // Basic widget type detection
    if (GTK_IS_LABEL(widget)) {
        set_widget_type(node, "label");
        
        // Get label text and add as property
        const char *text = gtk_label_get_text(GTK_LABEL(widget));
        add_property_to_node(node, "text_label", text);
        
        // Check if mnemonic is used
        gboolean use_underline = gtk_label_get_use_underline(GTK_LABEL(widget));
        add_property_to_node(node, "mnemonic", use_underline ? "1" : "0");
        
        // Add style properties
        add_style_property_to_node(node, "police", "Arial");
        add_style_property_to_node(node, "color", "black");
        add_style_property_to_node(node, "taille", "10");
        add_style_property_to_node(node, "gras", "0");
    }
    else if (GTK_IS_BUTTON(widget)) {
        set_widget_type(node, "button");
        
        // Get button label text
        const char *text = gtk_button_get_label(GTK_BUTTON(widget));
        add_property_to_node(node, "text", text ? text : "");
        
        // Set placeholder for icon
        add_property_to_node(node, "icon", "icon.png");
        
        // Check if mnemonic is used
        gboolean use_underline = gtk_button_get_use_underline(GTK_BUTTON(widget));
        add_property_to_node(node, "mnemonic", use_underline ? "1" : "0");
        
        // Add style properties
        add_style_property_to_node(node, "police", "Arial");
        add_style_property_to_node(node, "color", "white");
        add_style_property_to_node(node, "taille", "8");
        add_style_property_to_node(node, "gras", "1");
        add_style_property_to_node(node, "modern_bgcolor", "pink,blue");
    }
    else if (GTK_IS_ENTRY(widget)) {
        set_widget_type(node, "entry");
        
        // Get entry properties
        const char *placeholder = gtk_entry_get_placeholder_text(GTK_ENTRY(widget));
        gboolean is_visible = gtk_entry_get_visibility(GTK_ENTRY(widget));
        
        // Store properties
        add_property_to_node(node, "placeholder", placeholder ? placeholder : "");
        add_property_to_node(node, "icon", "icon.png");
        add_property_to_node(node, "visibility", is_visible ? "1" : "0");
        add_property_to_node(node, "carReplacement", "-");
        add_property_to_node(node, "alignement", "0");
        add_property_to_node(node, "maxCaracteres", "50");
        add_property_to_node(node, "setFrame", "1");
    }
    else if (GTK_IS_BOX(widget)) {
        set_widget_type(node, "box");
        
        // Get box properties
        GtkOrientation orientation = gtk_orientable_get_orientation(GTK_ORIENTABLE(widget));
        int spacing = gtk_box_get_spacing(GTK_BOX(widget));
        
        // Store properties
        add_property_to_node(node, "orientation", orientation == GTK_ORIENTATION_HORIZONTAL ? "h" : "v");
        char spacing_str[16];
        sprintf(spacing_str, "%d", spacing);
        add_property_to_node(node, "space_child", spacing_str);
        add_property_to_node(node, "homogene", "1");
        add_property_to_node(node, "bgColor", "pink");
    }
    
    // For all widgets, add position and size properties based on the container type
    GtkWidget *parent = gtk_widget_get_parent(widget);
    if (parent && GTK_IS_FIXED(parent)) {
        gint x = 0, y = 0;
        gtk_container_child_get(GTK_CONTAINER(parent), widget, "x", &x, "y", &y, NULL);
        
        char x_str[16], y_str[16];
        sprintf(x_str, "%d", x);
        sprintf(y_str, "%d", y);
        
        add_property_to_node(node, "x", x_str);
        add_property_to_node(node, "y", y_str);
    }
    
    // Add size properties if they have been specifically set
    gint width, height;
    gtk_widget_get_size_request(widget, &width, &height);
    if (width > 0 && height > 0) {
        char width_str[16], height_str[16];
        sprintf(width_str, "%d", width);
        sprintf(height_str, "%d", height);
        
        add_property_to_node(node, "width", width_str);
        add_property_to_node(node, "height", height_str);
    }
}

#endif /* WIDGET_PROPS_H */
