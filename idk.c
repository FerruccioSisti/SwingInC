#include <gtk/gtk.h>
#include <math.h>

static void do_drawing(cairo_t *);

int points[11][2] = {
    { 0, 85 },
    { 75, 75 },
    { 100, 10 },
    { 125, 75 },
    { 200, 85 },
    { 150, 125 },
    { 160, 190 },
    { 100, 150 },
    { 40, 190 },
    { 50, 125 },
    { 0, 85 }
};

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,
    gpointer user_data)
{
  do_drawing(cr);

  return FALSE;
}

static void do_drawing(cairo_t *cr)
{
    cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
      cairo_set_line_width(cr, 1);

      cairo_rectangle(cr, 20, 20, 120, 80);
      cairo_rectangle(cr, 180, 20, 80, 80);
      cairo_stroke_preserve(cr);
      cairo_fill(cr);

      cairo_arc(cr, 330, 60, 40, 0, 2*M_PI);
      cairo_stroke_preserve(cr);
      cairo_fill(cr);

      cairo_arc(cr, 90, 160, 40, M_PI/4, M_PI);
      cairo_close_path(cr);
      cairo_stroke_preserve(cr);
      cairo_fill(cr);

      cairo_translate(cr, 220, 180);
      cairo_scale(cr, 1, 0.7);
      cairo_arc(cr, 0, 0, 50, 0, 2*M_PI);
      cairo_stroke_preserve(cr);
      cairo_fill(cr);
  
}

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *darea;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);

  g_signal_connect(G_OBJECT(darea), "draw",
      G_CALLBACK(on_draw_event), NULL);
  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 460, 240);
  gtk_window_set_title(GTK_WINDOW(window), "Other shapes");

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
