/**
Author: Ferruccio Sisti
Resources used:
http://zetcode.com/gfx/cairo/shapesfills/
https://www.cairographics.org/tutorial/
https://www.cairographics.org/manual-1.0.2/
**/

#include <gtk/gtk.h>
#include <math.h>

// Surface to store current scribbles
static cairo_surface_t *surface = NULL;
//Default shape to 0 (represents rectangle)
int shape = 0;

//Function creates a blank canvas
void clear_surface (void)
{
  cairo_t *cr;

  cr = cairo_create (surface);

  //Sets colour to white (rgb values must be between 0 and 1)
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);

  //Free up resources used by cr
  cairo_destroy (cr);
}

//Function creates a window to work with
gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
    //Create a widget window
    surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                               CAIRO_CONTENT_COLOR,
                                               gtk_widget_get_allocated_width (widget),
                                               gtk_widget_get_allocated_height (widget));
    //Clear the surface (make it white)
    clear_surface ();
    return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
//Function redraws the screen from the surface
gboolean draw_cb (GtkWidget *widget, cairo_t *cr, gpointer data)
{
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);

  return FALSE;
}

// Draw a rectangle or circle depending on if parent or child
void draw_brush (GtkWidget *widget, gdouble x, gdouble y)
{
  cairo_t *cr;

  //Paint on surface
  cr = cairo_create (surface);
  //Set attributes
  cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
  cairo_set_line_width(cr, 1);

  //Depending on shape value, create a rectangle or circle of a predetermined size
  //At the cursor location when primary mouse button clicked
  if (shape == 0)
  {
      cairo_rectangle(cr, x - 210, y - 140, 420, 280);
      cairo_stroke_preserve(cr);
      cairo_fill(cr);
  }
  else if (shape == 1)
  {
      //Circles are made using cairo_arc because cairo_circle doesn't exist
      cairo_arc(cr, x, y, 180, 0, 2*M_PI);
      cairo_stroke_preserve(cr);
      cairo_fill(cr);
  }

  //Free up resources
  cairo_destroy (cr);
}

//Function clears the surface and draws a shape if the user left clicks surface
gboolean button_press_event_cb (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  //If no valid surface to draw on then don't do anything
  if (surface == NULL)
  {
      return FALSE;
  }

  //When user left clicks on surface
  if (event->button == GDK_BUTTON_PRIMARY)
  {
      //Clear the window so its blank before drawing a shape
      clear_surface();
      gtk_widget_queue_draw (widget);
      draw_brush (widget, event->x, event->y);
  }

  return TRUE;
}

//Function for when window is closed
void close_window (void)
{
    if (surface)
    {
        cairo_surface_destroy (surface);
    }
}

//Function sets up windows and connections
void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *frame;
  GtkWidget *drawing_area;

  //Create window for drawing
  window = gtk_application_window_new (app);
  if (shape == 0)
  {
      gtk_window_set_title (GTK_WINDOW (window), "Rectangle Window");
  }
  else
  {
      gtk_window_set_title (GTK_WINDOW (window), "Circle Window");
  }

  //Set up action for window termination
  g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

  //Set window attributes
  gtk_container_set_border_width (GTK_CONTAINER (window), 8);
  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_container_add (GTK_CONTAINER (window), frame);
  drawing_area = gtk_drawing_area_new ();
  gtk_widget_set_size_request (drawing_area, 1000, 700);
  gtk_container_add (GTK_CONTAINER (frame), drawing_area);

  // Setting up signals
  g_signal_connect (drawing_area, "draw", G_CALLBACK (draw_cb), NULL);
  g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);
  g_signal_connect (drawing_area, "button-press-event", G_CALLBACK (button_press_event_cb), NULL);

  //Set the events for the drawing area
  gtk_widget_set_events (drawing_area, gtk_widget_get_events (drawing_area)
                                     | GDK_BUTTON_PRESS_MASK
                                     | GDK_POINTER_MOTION_MASK);
  gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
  GtkApplication *app, *app2;
  GtkWidget *window;
  int status, status2;

  /**Fork before creating the window so we can have one that does rectangles
  and one that does circles **/
  if (fork() > 0)
  {
      app = gtk_application_new ("rectangle.example", G_APPLICATION_FLAGS_NONE);
      g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
      status = g_application_run (G_APPLICATION (app), argc, argv);
      g_object_unref (app);
  }
  else
  {
      //Change shape from rectangle to circle
      shape = 1;
      app2 = gtk_application_new ("circle.example", G_APPLICATION_FLAGS_NONE);
      g_signal_connect (app2, "activate", G_CALLBACK (activate), NULL);
      status2 = g_application_run (G_APPLICATION (app2), argc, argv);
      g_object_unref (app2);
  }

  return status;
}
