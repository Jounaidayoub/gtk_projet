#ifndef WIDGET_PROPS_H
#define WIDGET_PROPS_H

#include <gtk/gtk.h>
#include "containers_list.h"
#include "entry.h"
#include "widget_types.h"

// Helper function to populate widget properties in the Arbre node from widget structures
void populate_widget_properties(Arbre *node, void *widget_structure, WidgetType type) {
    // Set the widget type in the node
    node->type = type;
    
    // Store a pointer to the widget structure for later use
    node->widget_data = widget_structure;
    
    g_print("Populating widget properties for node %s, type %s, data: %p\n", 
           node->nom, widget_type_to_string(type), widget_structure);
    
    // Process based on widget type
    switch (type) {
        case WIDGET_ENTRY_BASIC:
        {
            // Cast to the correct structure type
            entry_type_basic *entry = (entry_type_basic*)widget_structure;
            
            if (entry) {
                // Add properties from the structure
                char width_str[16], height_str[16], x_str[16], y_str[16], max_len_str[16];
                sprintf(width_str, "%d", entry->dim->width);
                sprintf(height_str, "%d", entry->dim->height);
                sprintf(x_str, "%d", entry->cord->x);
                sprintf(y_str, "%d", entry->cord->y);
                sprintf(max_len_str, "%d", entry->maxlen);
                
                add_property_to_node(node, "width", width_str);
                add_property_to_node(node, "height", height_str);
                add_property_to_node(node, "x", x_str);
                add_property_to_node(node, "y", y_str);
                add_property_to_node(node, "editable", entry->is_editable ? "1" : "0");
                add_property_to_node(node, "visibility", entry->is_visible ? "1" : "0");
                add_property_to_node(node, "placeholder", entry->placeholder_text ? entry->placeholder_text : "");
                add_property_to_node(node, "maxCaracteres", max_len_str);
                add_property_to_node(node, "default_text", entry->default_text ? entry->default_text : "");
            }
            
            // add_property_to_node(node, "setFrame", "1");
            
            // // Add style properties
            // add_style_property_to_node(node, "police", "Arial");
            // add_style_property_to_node(node, "color", "black");
            // add_style_property_to_node(node, "taille", "10");
            // add_style_property_to_node(node, "gras", "0");
            break;
        }
        case WIDGET_ENTRY_PASSWORD:
        {
            // Cast to the correct structure type
            entry_type_password *entry = (entry_type_password*)widget_structure;
            
            if (entry) {
                // Add properties from the structure - only use fields that exist in entry_type_password
                char width_str[16], height_str[16], x_str[16], y_str[16];
                char invisible_char_str[2] = {entry->invisible_char, '\0'};
                
                sprintf(width_str, "%d", entry->dim->width);
                sprintf(height_str, "%d", entry->dim->height);
                sprintf(x_str, "%d", entry->cord->x);
                sprintf(y_str, "%d", entry->cord->y);
                
                add_property_to_node(node, "width", width_str);
                add_property_to_node(node, "height", height_str);
                add_property_to_node(node, "x", x_str);
                add_property_to_node(node, "y", y_str);
                
                // Password entries are always not visible, always editable
                add_property_to_node(node, "editable", "1");
                add_property_to_node(node, "visibility", "0");
                add_property_to_node(node, "placeholder", entry->placeholder_text ? entry->placeholder_text : "");
                add_property_to_node(node, "maxCaracteres", "50"); // Default value
                add_property_to_node(node, "invisible_char", invisible_char_str);
                add_property_to_node(node, "default_text", ""); // No default text for security
            }
            
            // add_property_to_node(node, "setFrame", "1");
            
            // // Add style properties
            // add_style_property_to_node(node, "police", "Arial");
            // add_style_property_to_node(node, "color", "black");
            // add_style_property_to_node(node, "taille", "10");
            // add_style_property_to_node(node, "gras", "0");
            break;
        }
        // Add more cases for other widget types as needed
        default:
            break;
    }
}

#endif /* WIDGET_PROPS_H */
