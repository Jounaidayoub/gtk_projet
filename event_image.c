#include <gtk/gtk.h>

//compile with cc `pkg-config --cflags gtk+-3.0` text.c `pkg-config --libs gtk+-3.0` -o text

static void click_callback(GtkWidget *widget, GdkEventButton *event, gpointer );
static gboolean inRange(gint value, gint min, gint max);
static gboolean pointInRect(gint mouseX, gint mouseY, gint wx, gint wy, gint width, gint height);

GtkWidget *image;
GtkWidget *image2;
   
int main (int argc, char *argv[])
{
  GtkWidget *event_box;
  GtkWidget *window;
  GtkWidget *box;
 
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  
  box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  /* GTK_ALIGN_CENTER is necessary otherwise 
   * the callback triggers from the space on
   * the top and the bottom of the image */
   
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
  
  image  = gtk_image_new_from_file("icon.png");
  image2 = gtk_image_new_from_file("ex_image.jpg");
  gtk_box_pack_start(GTK_BOX(box), image, TRUE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), image2, TRUE, FALSE, 0);
  
  event_box = gtk_event_box_new ();
  gtk_container_add (GTK_CONTAINER (event_box), box);

  gtk_container_add (GTK_CONTAINER (window), event_box);
  
  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect( event_box, "button_press_event", G_CALLBACK( click_callback ), image);
  
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Image click event");

  gtk_widget_show_all(window);

 gtk_main();

  return 0;
}

static void click_callback(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    GtkAllocation alloc;
    GtkAllocation alloc2;

    gtk_widget_get_allocation(image, &alloc);
    gtk_widget_get_allocation(image2, &alloc2);
    
    if (pointInRect(event->x, event->y, alloc.x, alloc.y, alloc.width, alloc.height))
        g_print("You clicked from image\n");

    if (pointInRect(event->x, event->y, alloc2.x, alloc2.y, alloc2.width, alloc2.height))
        g_print("You clicked from image2\n");

}

static gboolean pointInRect(gint mouseX, gint mouseY, gint wx, gint wy, gint width, gint height)
{
    return inRange(mouseX, wx, wx + width) && 
            inRange(mouseY, wy, wy + height);
}

static gboolean inRange(gint value, gint min, gint max)
{
    return (value >= min && value <= max);
}