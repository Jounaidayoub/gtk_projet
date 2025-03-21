#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>
#include <stdio.h>
#include "app_data.h"
#include "widget_types.h"
#include "entry.h"

// Function to handle drag data received
static void on_drag_data_received(GtkWidget *widget, GdkDragContext *context, gint x, gint y,
                                  GtkSelectionData *data, guint info, guint time, gpointer user_data)
{
    const gchar *widget_type = (const gchar *)gtk_selection_data_get_data(data);
    GtkWidget *new_widget = NULL;

    if (g_strcmp0(widget_type, "Label") == 0)
    {
        new_widget = gtk_label_new("New Label");
    }
    else if (g_strcmp0(widget_type, "Button") == 0)
    {
        new_widget = gtk_button_new_with_label("New Button");
    }
    else if (g_strcmp0(widget_type, "Entry") == 0)
    {
        new_widget = gtk_entry_new();
    }
    else if (g_strcmp0(widget_type, "ComboBox") == 0)
    {
        new_widget = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(new_widget), "Option 1");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(new_widget), "Option 2");
    }
    else if (g_strcmp0(widget_type, "Scale") == 0)
    {
        new_widget = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    }

    if (new_widget)
    {
        gtk_grid_attach(GTK_GRID(widget), new_widget, x / 100, y / 50, 1, 1);
        gtk_widget_show_all(widget);
    }

    gtk_drag_finish(context, TRUE, FALSE, time);
}

// Updated function to generate XML from structures stored in Arbre nodes
void generate_xml_from_arbre(GString *string, Arbre *racine, int indent)
{
    if (!racine)
        return;

    // Skip preview_area node which is just a container
    if (strcmp(racine->nom, "preview_area") == 0)
    {
        // Just process its children
        if (racine->fils)
        {
            generate_xml_from_arbre(string, racine->fils, indent);
        }
        return;
    }

    // Debug info
    g_print("Processing node: %s, type: %s, has widget_data: %s\n",
            racine->nom,
            widget_type_to_string(racine->type),
            racine->widget_data ? "YES" : "NO");

    // Output indentation
    for (int i = 0; i < indent; i++)
        g_string_append(string, "  ");

    // Get widget type from the enum
    const char *widget_type = widget_type_to_string(racine->type);

    // Output element start
    // g_string_append_printf(string, "<%s>\n", widget_type);

    // Output properties directly from structure based on widget type
    if (racine->widget_data)
    {
        g_print("\n\n racine type: %d", racine->type);
        switch (racine->type)
        {
        case WIDGET_TEXTVIEW:
        {   
            
            MonTextView *textview = (MonTextView *)racine->widget_data;
            
            // Open TextView tag
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "<TextView>\n");
            
            // Text property
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"text\" >%s</property>\n", 
                                  textview->texte ? textview->texte : "");
            
            // Position properties
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"x\" >%d</property>\n", textview->Crd.x);
            
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"y\" >%d</property>\n", textview->Crd.y);
            
            // Size properties
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"width\" >%d</property>\n", textview->dim.width);
            
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"height\" >%d</property>\n", textview->dim.height);
            
            // Close TextView tag
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "</TextView>\n");
            
            break;
        }
            // BASIC ENTRY - Fix to match sample XML format
        case WIDGET_ENTRY_BASIC:
        {
            entry_type_basic *entry = (entry_type_basic *)racine->widget_data;
            g_print("  Entry basic properties - w:%d h:%d x:%d y:%d\n",
                    entry->dim->width, entry->dim->height,
                    entry->cord->x, entry->cord->y);

            // Open entry tag
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "<entry>\n");

            // Properties in the same order as your sample
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"type\" >basic</property>\n");

            if (entry->placeholder_text)
            {
                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"placeholder\" >%s</property>\n",
                                       entry->placeholder_text);
            }

            // Visibility property renamed to match sample
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"visible\" >%d</property>\n", entry->is_visible);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"editable\" >%d</property>\n", entry->is_editable);

            // Position and size properties
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"x\" >%d</property>\n", entry->cord->x);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"y\" >%d</property>\n", entry->cord->y);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"width\" >%d</property>\n", entry->dim->width);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"height\" >%d</property>\n", entry->dim->height);

            // maxCaracteres renamed to max to match sample
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"max\" >%d</property>\n", entry->maxlen);

            if (entry->default_text)
            {
                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"default_text\" >%s</property>\n",
                                       entry->default_text);
            }

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "</entry>\n");
            break;
        }

        // PASSWORD ENTRY - Fix to match sample XML format
        case WIDGET_ENTRY_PASSWORD:
        {
            entry_type_password *entry = (entry_type_password *)racine->widget_data;
            g_print("  Entry password properties - w:%d h:%d x:%d y:%d\n",
                    entry->dim->width, entry->dim->height,
                    entry->cord->x, entry->cord->y);

            // Open entry tag
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "<entry>\n");

            // Properties in the same order as your sample
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"type\" >pass</property>\n");

            if (entry->placeholder_text)
            {
                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"placeholder\" >%s</property>\n",
                                       entry->placeholder_text);
            }

            // Add visible and editable properties
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"visible\" >1</property>\n");

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"editable\" >1</property>\n");

            // Position and size properties
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"x\" >%d</property>\n", entry->cord->x);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"y\" >%d</property>\n", entry->cord->y);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"width\" >%d</property>\n", entry->dim->width);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"height\" >%d</property>\n", entry->dim->height);

            // Add max property to match sample (using a default value or from struct if available)
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"max\" >50</property>\n"); // Use maxlen if available or default to 50

            // Optional: keep invisible_char if needed for your application
            // for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
            // g_string_append_printf(string, "<property name=\"invisible_char\" >%c</property>\n",
            //                      entry->invisible_char);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "</entry>\n");
            break;
        }

        // Normal Button
        // case WIDGET_BUTTON_NORMAL:
        // {
        //     btn *button = (btn*)racine->widget_data;
        //     g_print("  Button normal properties - x:%d y:%d\n",
        //            button->pos ? button->pos->x : 0,
        //            button->pos ? button->pos->y : 0);

        //     // Position and size properties
        //     if (button->pos) {
        //         for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
        //         g_string_append(string, "<button>\n");

        //         for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
        //         g_string_append_printf(string, "<property name=\"x\">%d</property>\n", button->pos->x);

        //         for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
        //         g_string_append_printf(string, "<property name=\"y\">%d</property>\n", button->pos->y);
        //     }

        //     if (button->dim) {
        //         for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
        //         g_string_append_printf(string, "<property name=\"width\">%d</property>\n", button->dim->width);

        //         for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
        //         g_string_append_printf(string, "<property name=\"height\">%d</property>\n", button->dim->height);
        //     }

        //     // Button-specific properties
        //     if (button->label) {
        //         for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
        //         g_string_append_printf(string, "<property name=\"text\">%s</property>\n", button->label);
        //     }

        //     // if (button->tooltip) {
        //     //     for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
        //     //     g_string_append_printf(string, "<property name=\"tooltip\">%s</property>\n", button->tooltip);
        //     // }
        //     for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
        //     g_string_append(string, "</button>\n");
        //     break;
        // }

        // Checkbox Button
        case WIDGET_CHECKBOX:
        {
            btn *button = (btn *)racine->widget_data;
            g_print("  Checkbox properties - x:%d y:%d checked:%d\n",
                    button->pos ? button->pos->x : 0,
                    button->pos ? button->pos->y : 0,
                    button->isChecked);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "<checkbox>\n");
            // Position and size properties
            if (button->pos)
            {
                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"x\" >%d</property>\n", button->pos->x);

                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"y\" >%d</property>\n", button->pos->y);
            }

            // Checkbox-specific properties
            if (button->label)
            {
                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"text\" >%s</property>\n", button->label);
            }

            // if (button->tooltip)
            // {
            //     for (int j = 0; j < indent + 2; j++)
            //         g_string_append(string, "  ");
            //     g_string_append_printf(string, "<property name=\"tooltip\">%s</property>\n", button->tooltip);
            // }

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"checked\" >%d</property>\n", button->isChecked);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "</checkbox>\n");
            break;
        }

        // Radio Button
        /*
        case WIDGET_BUTTON_RADIO:
        {
            btn *button = (btn*)racine->widget_data;
            g_print("  Radio button properties - x:%d y:%d\n",
                   button->pos ? button->pos->x : 0,
                   button->pos ? button->pos->y : 0);

            // Position and size properties
            if (button->pos) {
                for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"x\">%d</property>\n", button->pos->x);

                for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"y\">%d</property>\n", button->pos->y);
            }

            // Radio button-specific properties
            if (button->label) {
                for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"label\">%s</property>\n", button->label);
            }

            if (button->tooltip) {
                for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"tooltip\">%s</property>\n", button->tooltip);
            }

            // if (button->groupe) {
            //     for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
            //     g_string_append_printf(string, "<property name=\"group\">%s</property>\n", button->groupe);
            // }

            break;
        }
         */
        // Toggle Button
        case WIDGET_BUTTON_TOGGLE:
        {
            btn *button = (btn *)racine->widget_data;
            g_print("  Toggle button properties - x:%d y:%d checked:%d\n",
                    button->pos ? button->pos->x : 0,
                    button->pos ? button->pos->y : 0,
                    button->isChecked);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "<button>\n");
            // Position and size properties
            if (button->pos)
            {
                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"x\">%d</property>\n", button->pos->x);

                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"y\">%d</property>\n", button->pos->y);
            }

            if (button->dim)
            {
                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"width\">%d</property>\n", button->dim->width);

                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"height\">%d</property>\n", button->dim->height);
            }

            // Toggle button-specific properties
            if (button->label)
            {
                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"label\">%s</property>\n", button->label);
            }

            if (button->tooltip)
            {
                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"tooltip\">%s</property>\n", button->tooltip);
            }

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"is_active\">%d</property>\n", button->isChecked);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "</button>\n");

            break;
        }

        // Switch Button
        case WIDGET_BUTTON_SWITCH:
        {
            btn *button = (btn *)racine->widget_data;
            g_print("  Switch button properties - x:%d y:%d active:%d\n",
                    button->pos ? button->pos->x : 0,
                    button->pos ? button->pos->y : 0,
                    button->isChecked);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "<button>\n");

            // Position and size properties
            if (button->pos)
            {
                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"x\">%d</property>\n", button->pos->x);

                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"y\">%d</property>\n", button->pos->y);
            }

            if (button->dim)
            {
                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"width\">%d</property>\n", button->dim->width);

                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"height\">%d</property>\n", button->dim->height);
            }

            // Switch-specific properties
            if (button->tooltip)
            {
                for (int j = 0; j < indent + 2; j++)
                    g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"tooltip\">%s</property>\n", button->tooltip);
            }

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"is_active\">%d</property>\n", button->isChecked);

            // Close </radio> tag
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "</button>\n");

            break;
        }

            // Spin Button
            // Spin Button
            // case WIDGET_BUTTON_SPIN:
            // {
            // btn *button = (btn *)racine->widget_data;

            // // Get the spinObj data safely
            // spinObj *sp = button->sp;
            // if (!sp) {
            //     g_print("  Spin button missing spinObj data, using defaults\n");
            //     // Create default spin object if missing
            //     sp = g_malloc(sizeof(spinObj));
            //     sp->borneInf = 0.0;
            //     sp->borneSup = 100.0;
            //     sp->step = 1.0;
            //     sp->digits = 0;
            //     sp->start = 0.0;
            // }

            // g_print("  Spin button properties - x:%d y:%d min:%f max:%f step:%f digits:%u start:%f\n",
            //         button->pos ? button->pos->x : 0,
            //         button->pos ? button->pos->y : 0,
            //         sp->borneInf, sp->borneSup, sp->step, sp->digits, sp->start);

            // // Changed from <button> to <spin> to match parser's expected format
            // for (int j = 0; j < indent + 2; j++)
            //     g_string_append(string, "  ");
            // g_string_append(string, "<spin>\n");

            // // Add type property to match parser expectations
            // for (int j = 0; j < indent + 2; j++)
            //     g_string_append(string, "  ");
            // g_string_append_printf(string, "<property name=\"type\" >numeric</property>\n");

            // // Position and size properties
            // if (button->pos)
            // {
            //     for (int j = 0; j < indent + 2; j++)
            //         g_string_append(string, "  ");
            //     g_string_append_printf(string, "<property name=\"x\" >%d</property>\n", button->pos->x);

            //     for (int j = 0; j < indent + 2; j++)
            //         g_string_append(string, "  ");
            //     g_string_append_printf(string, "<property name=\"y\" >%d</property>\n", button->pos->y);
            // }

            // if (button->dim)
            // {
            //     for (int j = 0; j < indent + 2; j++)
            //         g_string_append(string, "  ");
            //     g_string_append_printf(string, "<property name=\"width\" >%d</property>\n", button->dim->width);

            //     for (int j = 0; j < indent + 2; j++)
            //         g_string_append(string, "  ");
            //     g_string_append_printf(string, "<property name=\"height\" >%d</property>\n", button->dim->height);
            // }

            // // Spin button-specific properties - align names with parser expectations
            // if (button->tooltip)
            // {
            //     for (int j = 0; j < indent + 2; j++)
            //         g_string_append(string, "  ");
            //     g_string_append_printf(string, "<property name=\"tooltip\" >%s</property>\n", button->tooltip);
            // }

            // // Spin-specific properties from spinObj - match parser property names
            // for (int j = 0; j < indent + 2; j++)
            //     g_string_append(string, "  ");
            // g_string_append_printf(string, "<property name=\"min\" >%g</property>\n", sp->borneInf);

            // for (int j = 0; j < indent + 2; j++)
            //     g_string_append(string, "  ");
            // g_string_append_printf(string, "<property name=\"max\" >%g</property>\n", sp->borneSup);

            // for (int j = 0; j < indent + 2; j++)
            //     g_string_append(string, "  ");
            // g_string_append_printf(string, "<property name=\"step\" >%g</property>\n", sp->step);

            // for (int j = 0; j < indent + 2; j++)
            //     g_string_append(string, "  ");
            // g_string_append_printf(string, "<property name=\"digits\" >%u</property>\n", sp->digits);

            // for (int j = 0; j < indent + 2; j++)
            //     g_string_append(string, "  ");
            // g_string_append_printf(string, "<property name=\"value\" >%g</property>\n", sp->start);

            // // Close spin tag
            // for (int j = 0; j < indent + 2; j++)
            //     g_string_append(string, "  ");
            // g_string_append(string, "</spin>\n");

            // // Free the temporary spin object if we created one
            // if (!button->sp && sp) {
            //     g_free(sp);
            // }

            // break;
            // }

        case WIDGET_RADIO:
        {
            g_print("\ndans le case du radio");
            btn *radio = (btn *)racine->widget_data;
            // g_print("\n\n\n==Radio widget %d", radio->dim->width);
            // g_print("Radio widget properties - text: %s, mnemonic: %d, police: %s, color: %s, taille: %d, gras: %d\n",
            //         radio->label, radio->hasMnemonic, radio->style->police, radio->style->color, radio->style->taille, radio->style->gras);

            // Open <radio> tag
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "<radio>\n");

            // Generate properties inside <radio>
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"text\" >%s</property>\n", radio->label);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"mnemonic\" >%d</property>\n", radio->hasMnemonic);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"police\" >%s</property>\n", radio->police);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"color\" >%s</property>\n", radio->color);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"taille\" >%d</property>\n", radio->taille);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"gras\" >%d</property>\n", radio->gras);

            // Close </radio> tag
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "</radio>\n");

            break;
        }

        case WIDGET_BUTTON:
        {
            btn *button = (btn *)racine->widget_data;
            // // Open <button> tag
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "<button>\n");

            // Generate properties inside <button>
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"nom\" >%s</property>\n", button->nom);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"text\" >%s</property>\n", button->label);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"mnemonic\" >%d</property>\n", button->hasMnemonic);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"police\" >%s</property>\n", button->police);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"color\" >%s</property>\n", button->color);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"taille\" >%d</property>\n", button->taille);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"x\" >%d</property>\n", button->pos->x);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"y\" >%d</property>\n", button->pos->y);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"width\" >%d</property>\n", button->dim->width);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"height\" >%d</property>\n", button->dim->height);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<property name=\"gras\" >%d</property>\n", button->gras);

            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
                g_string_append_printf(string, "<property name=\"bgcolor\" >%s</property>\n", button->bgcolor);

            // // Close </button> tag
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "</button>\n");

            break;
        }

        case WIDGET_RADIO_LIST:
        {
            StyledBox *radioList = (StyledBox *)racine->widget_data;

            // Generate the opening tag for <radioList> with x and y attributes
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append_printf(string, "<radioList x=%d y=%d >\n", radioList->cord->x, radioList->cord->y);

            // generate the radio buttons
            generate_xml_from_arbre(string, racine->fils, indent + 4);

            // Close the <radioList> tag
            for (int j = 0; j < indent + 2; j++)
                g_string_append(string, "  ");
            g_string_append(string, "</radioList>\n");

            break;
        }

            /// add cases for ur widgets here

        default:
            g_print("  Unknown widget type: %d\n", racine->type);
            break;
        }
    }
    else
    {
        g_print("  No widget_data present\n");
    }

    // Also output any properties directly stored in the Arbre node
    // for (int i = 0; i < racine->prop_count; i++) {
    //     for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");
    //     g_string_append_printf(string, "<property name=\"%s\">%s</property>\n",
    //                           racine->properties[i].name, racine->properties[i].value);
    // }

    // Output style properties if available
    // for (int i = 0; i < racine->style_prop_count; i++) {
    //     for (int j = 0; j < indent + 2; j++) g_string_append(string, "  ");

    //     // Style properties vary by widget type
    //     const char *style_tag = "style";
    //     if (strcmp(widget_type, "label") == 0) {
    //         style_tag = "labelstyle";
    //     } else if (strcmp(widget_type, "button") == 0) {
    //         style_tag = "buttonstyle";
    //     } else if (strcmp(widget_type, "checkbox") == 0) {
    //         style_tag = "checkboxstyle";
    //     }

    //     g_string_append_printf(string, "<%s name=\"%s\">%s</%s>\n",
    //                           style_tag, racine->style_props[i].name,
    //                           racine->style_props[i].value, style_tag);
    // }

    // // Handle children for containers
    // if (racine->fils) {
    //     // If this is a container other than preview_area, wrap children in a children tag
    //     if (racine->is_container) {
    //         for (int i = 0; i < indent + 2; i++) g_string_append(string, "  ");
    //         g_string_append(string, "<child>\n");

    //         generate_xml_from_arbre(string, racine->fils, indent + 4);

    //         for (int i = 0; i < indent + 2; i++) g_string_append(string, "  ");
    //         g_string_append(string, "</child>\n");
    //     } else {
    //         generate_xml_from_arbre(string, racine->fils, indent + 2);
    //     }
    // }

    // Process siblings at the same level
    if (racine->frere)
    {
        // Close current tag before moving to sibling
        // for (int i = 0; i < indent; i++) g_string_append(string, "  ");
        // g_string_append_printf(string, "</%s>\n", widget_type);

        generate_xml_from_arbre(string, racine->frere, indent);
        return; // Skip the closing tag at the end since we already added it
    }

    // Close element
    // commented because each element should close itself
    // for (int i = 0; i < indent; i++) g_string_append(string, "  ");
    // g_string_append_printf(string, "</%s>\n", widget_type);
}

// Export to XML function
void export_to_xml(GtkWidget *widget, gpointer data)
{
    AppData *app_data = (AppData *)data;

    // Open a memory stream to write XML
    GString *xml_string = g_string_new("");

    // Start immediately with the window tag without XML declaration
    g_string_append(xml_string, "<window>\n");

    // Add window properties to match d.html structure
    g_string_append(xml_string, "    <property name=\"title\" >GTK UI Builder</property>\n");
    g_string_append(xml_string, "    <property name=\"width\" >900</property>\n");
    g_string_append(xml_string, "    <property name=\"height\" >900</property>\n");
    g_string_append(xml_string, "    <property name=\"resizable\" >1</property>\n");
    g_string_append(xml_string, "    <property name=\"border\" >10</property>\n");
    g_string_append(xml_string, "    <property name=\"position\" >c</property>\n");
    g_string_append(xml_string, "    <property name=\"x\" >400</property>\n");
    g_string_append(xml_string, "    <property name=\"y\" >250</property>\n");
    // g_string_append(xml_string, "    <property name=\"bgColor\" >#ffffff</property>\n");
    g_string_append(xml_string, "    <property name=\"icon\" >icon.png</property>\n");
    // g_string_append(xml_string, "    <property name=\"headerBar\" >1</property>\n");
    // g_string_append(xml_string, "    <property name=\"bgImage\" >background.png</property>\n");
    g_string_append(xml_string, "    <child>\n\n");
    g_string_append(xml_string, "        <fixed>\n\n");
    g_string_append(xml_string, "            <children>\n");

    // Generate XML for all widgets in the tree
    if (app_data->widget_tree)
    {
        generate_xml_from_arbre(xml_string, app_data->widget_tree, 4);
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

    // Save to file demo.html
    FILE *file = fopen("demo.html", "w");
    if (file)
    {
        fputs(xml_string->str, file);
        fclose(file);
        g_print("XML exported to demo.html\n");
    }
    else
    {
        g_print("Error: Could not save to demo.html\n");
    }

    // // Free the GString
    // g_string_free(xml_string, TRUE);

    //     GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(widget),
    //     GTK_DIALOG_DESTROY_WITH_PARENT,
    //     GTK_MESSAGE_INFO,
    //     GTK_BUTTONS_OK,
    //     "Running demo from demo.html");
    //     gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
    //         "Running demo from demo.html");

    // // Show the dialog without running it yet
    // gtk_widget_show(dialog);

    // // Allow the UI to update
    // while (gtk_events_pending())
    //     gtk_main_iteration();

    // // Check if demo.html exists
    // FILE *check = fopen("demo.html", "r");
    // if (!check) {
    //     gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
    //                                            "Error: Could not find demo.html");
    //     gtk_dialog_run(GTK_DIALOG(dialog));
    //     gtk_widget_destroy(dialog);
    //     return;
    // }
    // fclose(check);

    // // Copy to tst.html
    // // FILE *source = fopen("demo.html", "r");
    // // FILE *dest = fopen("tst.html", "w");

    // // if (source && dest) {
    // //     char buffer[1024];
    // //     size_t bytes;

    // //     // Copy the file content
    // //     while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
    // //         fwrite(buffer, 1, bytes, dest);
    // //     }

    //     // fclose(source);
    //     // fclose(dest);

    //     // Run the script or main executable
    //     // int status;
    //     // if (g_file_test("run_demo.sh", G_FILE_TEST_EXISTS)) {
    //     //     system("chmod +x run_demo.sh");
    //     //     status = system("./run_demo.sh");
    //     // } else {
    //     //     status = system("./build.sh xml_generator.c");
    //     //     if (status != 0) {
    //     //         status = system("./main.exe");
    //     //     }
    //     // }

    //     // if (status != 0) {
    //         // gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
    // //                                                "Error: Demo execution failed");
    // //     } else {
    // //         gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
    // //                                                "Demo executed successfully");
    // //     }
    // // } else {
    // //     gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
    // //                                            "Error: Could not copy demo.html to tst.html");
    // //     if (source) fclose(source);
    // //     if (dest) fclose(dest);
    // // }

    // Wait for user acknowledgment
    // gtk_dialog_run(GTK_DIALOG(dialog));

    // Destroy the dialog
    // gtk_widget_destroy(dialog);
}

// Update UI in response to a new container being selected
static void update_ui_for_container(AppData *app_data)
{
    GtkWidget *container = app_data->selected_container;

    if (!container)
    {
        // Default case, show widgets for the preview area
        g_print("Showing widgets for preview area\n");
    }
    else
    {
        // Show widgets for the selected container
        const gchar *name = gtk_widget_get_name(container);
        g_print("Showing widgets for container: %s\n", name);
    }
}

#endif /* CALLBACKS_H */
