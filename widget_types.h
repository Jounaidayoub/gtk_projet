#ifndef WIDGET_TYPES_H
#define WIDGET_TYPES_H

#include <string.h>

// Widget type enumeration
typedef enum {
    WIDGET_UNKNOWN = 0,
    WIDGET_WINDOW,
    WIDGET_BOX,
    WIDGET_FIXED,
    WIDGET_GRID,
    WIDGET_FRAME,
    WIDGET_SCROLLED_WINDOW,
    WIDGET_LABEL,
    WIDGET_BUTTON,
    WIDGET_ENTRY_BASIC,
    WIDGET_ENTRY_PASSWORD,
    WIDGET_COMBOBOX,
    WIDGET_SCALE,
    WIDGET_CHECKBOX,
    WIDGET_RADIO,
    WIDGET_SWITCH,
    WIDGET_SPINNER,
    WIDGET_PROGRESS_BAR,
    WIDGET_RADIO_LIST,
    WIDGET_BUTTON_NORMAL,
    WIDGET_BUTTON_CHECKBOX,
    WIDGET_BUTTON_RADIO,
    WIDGET_BUTTON_TOGGLE,
    WIDGET_BUTTON_SWITCH,
    WIDGET_BUTTON_SPIN,
    // Add more as needed
} WidgetType;

// Function declarations
const char* widget_type_to_string(WidgetType type);
WidgetType string_to_widget_type(const char* string);

// Implementation of widget_type_to_string directly in the header
const char* widget_type_to_string(WidgetType type) {
    switch (type) {
        case WIDGET_WINDOW: return "window";
        case WIDGET_BOX: return "box";
        case WIDGET_FIXED: return "fixed";
        case WIDGET_GRID: return "grid";
        case WIDGET_FRAME: return "frame";
        case WIDGET_SCROLLED_WINDOW: return "scrolledwindow";
        case WIDGET_LABEL: return "label";
        case WIDGET_BUTTON: return "button";
        case WIDGET_ENTRY_BASIC: return "entry";
        case WIDGET_ENTRY_PASSWORD: return "password";
        case WIDGET_COMBOBOX: return "combobox";
        case WIDGET_SCALE: return "scale";
        case WIDGET_CHECKBOX: return "checkbox";
        case WIDGET_RADIO: return "radio";
        case WIDGET_SWITCH: return "switch";
        case WIDGET_SPINNER: return "spinner";
        case WIDGET_PROGRESS_BAR: return "progressbar";
        case WIDGET_RADIO_LIST: return "radioList";
        case WIDGET_BUTTON_NORMAL: return "button";
        case WIDGET_BUTTON_CHECKBOX: return "checkbox";
        case WIDGET_BUTTON_RADIO: return "radio";
        case WIDGET_BUTTON_TOGGLE: return "toggle";
        case WIDGET_BUTTON_SWITCH: return "switch";
        case WIDGET_BUTTON_SPIN: return "spin";
        
        default: return "unknown";
    }
}

// Implementation of string_to_widget_type directly in the header
WidgetType string_to_widget_type(const char* string) {
    if (!string) return WIDGET_UNKNOWN;
    
    if (strcmp(string, "window") == 0) return WIDGET_WINDOW;
    if (strcmp(string, "box") == 0) return WIDGET_BOX;
    if (strcmp(string, "fixed") == 0) return WIDGET_FIXED;
    if (strcmp(string, "grid") == 0) return WIDGET_GRID;
    if (strcmp(string, "frame") == 0) return WIDGET_FRAME;
    if (strcmp(string, "scrolledwindow") == 0) return WIDGET_SCROLLED_WINDOW;
    if (strcmp(string, "label") == 0) return WIDGET_LABEL;
    if (strcmp(string, "button") == 0) return WIDGET_BUTTON;
    if (strcmp(string, "entry") == 0) return WIDGET_ENTRY_BASIC;
    if (strcmp(string, "password") == 0) return WIDGET_ENTRY_PASSWORD;
    if (strcmp(string, "combobox") == 0) return WIDGET_COMBOBOX;
    if (strcmp(string, "scale") == 0) return WIDGET_SCALE;
    if (strcmp(string, "checkbox") == 0) return WIDGET_CHECKBOX;
    if (strcmp(string, "radio") == 0) return WIDGET_RADIO;
    if (strcmp(string, "switch") == 0) return WIDGET_SWITCH;
    if (strcmp(string, "spinner") == 0) return WIDGET_SPINNER;
    if (strcmp(string, "progressbar") == 0) return WIDGET_PROGRESS_BAR;
    if (strcmp(string, "radioList") == 0) return WIDGET_RADIO_LIST;
    
    return WIDGET_UNKNOWN;
}

#endif /* WIDGET_TYPES_H */
