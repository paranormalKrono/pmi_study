//#include "MathPMI_Debug.h"
//#include <math.h>
//#include <gtk/gtk.h>
//#include <time.h>
//
//#include <signal.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <stdlib.h>
//#include <stdbool.h>
//#include <string.h>
//#include <errno.h>
//#include <ctype.h>
//#include <corecrt_math_defines.h>
//
//#define WIDTH   1500
//#define HEIGHT  800
//
//#define ZOOM_X  100.0
//#define ZOOM_Y  100.0
//
//void print_data()
//{
//	g_print("\n");
//}
//
//cairo_surface_t* surface = NULL;
//
//void clear_surface(void)
//{
//	cairo_t* cr;
//
//	cr = cairo_create(surface);
//
//	cairo_set_source_rgb(cr, 1, 1, 1);
//	cairo_paint(cr);
//
//	cairo_destroy(cr);
//}
//
//void configure_event_cb(GtkWidget* widget)
//{
//	if (surface)
//		cairo_surface_destroy(surface);
//
//	surface = gdk_surface_create_similar_surface(surface,
//		CAIRO_CONTENT_COLOR,
//		gtk_widget_get_allocated_width(widget),
//		gtk_widget_get_allocated_height(widget));
//
//	clear_surface();
//}
//
//void draw_cb(cairo_t* cr)
//{
//	cairo_set_source_surface(cr, surface, 0, 0);
//	cairo_paint(cr);
//}
//
//void draw_graph()
//{
//	int x;
//	int y = HEIGHT / 2;
//	int i, j;
//
//	cairo_t* cr;
//
//	cr = cairo_create(surface);
//	cairo_set_line_width(cr, 1);
//
//	srand(time(NULL));
//
//	for (x = 0; x < 1500; x++)
//	{
//		i = 10 % 6 % 3;
//		j = 10 % 6 % 3;
//		cairo_move_to(cr, x, y);
//
//		y += i - j;
//
//		cairo_line_to(cr, x + 1, y);
//	}
//
//	cairo_stroke(cr);
//
//	cairo_destroy(cr);
//}
//
//void draw_lines()
//{
//	clear_surface();
//
//	cairo_t* cr;
//
//	cr = cairo_create(surface);
//
//	/* Рисуем верхнюю линию */
//	cairo_set_source_rgba(cr, 0, 1, 0, 1);
//	cairo_move_to(cr, 0, (HEIGHT / 2) / 100 * 20);
//	cairo_line_to(cr, WIDTH, (HEIGHT / 2) / 100 * 20);
//	cairo_stroke(cr);
//
//	/* Рисуем нижнюю линию */
//	cairo_set_source_rgba(cr, 1, 0, 0, 1);
//	cairo_move_to(cr, 0, HEIGHT / 100 * 90);
//	cairo_line_to(cr, WIDTH, HEIGHT / 100 * 90);
//	cairo_stroke(cr);
//}
//
//int x = 0, y = HEIGHT / 2;
//int i, j;
//
//gboolean draw_graph2(gpointer widget)
//{
//	cairo_t* cr;
//
//	cr = cairo_create(surface);
//	cairo_set_line_width(cr, 1);
//
//	srand(time(NULL));
//
//	if (x < 1500)
//	{
//		i = rand() % 6 % 3;
//		j = rand() % 6 % 3;
//
//		cairo_move_to(cr, x, y);
//
//		y += i - j;
//
//		cairo_line_to(cr, x + 1, y);
//
//		x++;
//	}
//
//	cairo_stroke(cr);
//
//	gtk_widget_queue_draw(widget);
//
//	return TRUE;
//}
//
//
//gboolean	clock_timer_handler();
//double		second = 0;
//double		minute = 0;
//double		hour = 0;
//char		time_string[128];
//
//
////-----------------------------
////	main timer process
////-----------------------------
//
//gboolean	clock_timer_handler() { // check pulse every second
//
//	struct tm* tod;
//	time_t tx = time(NULL);		// get time of day`
//
//	tod = localtime(&tx);		// get time structure pointer
//
//	strcpy(time_string, asctime(tod));	// ASCII string of time
//	time_string[strlen(time_string) - 1] = 0; // char at end
//
//	second = tod->tm_sec;		// extract seconds
//	minute = tod->tm_min;		// extract minutes
//	hour = tod->tm_hour % 12;	// extract hour (12)
//
//	gtk_widget_queue_draw(draw2);	// trigger a draw event
//	return TRUE;			// timer keeps running (TRUE)
//}
//
////--------------------------
////	draw meter
////--------------------------
//
//void	needles(cairo_t* cr, double hor, double ver, double needle_length, double needle_position, double meter_radius, int cFlg) {
//
//	double x, x1, x2, y, y1, y2;
//	ver = ver + 1.0;
//	double X;
//
//	cairo_set_source_rgb(cr, 0.5, 0.5, 1.0); // blue
//
//	cairo_set_line_width(cr, 2.5);
//
//	//--------------------------------------------------------
//	//	One quarter of a circle (2 * M_PI -> full circle)
//	//--------------------------------------------------------
//
//	X = M_PI / 2.0;
//
//	for (int i = 0; i < 60; i++) { // all long hash marks
//
//		//		--------------------------------------------------------
//		//		Put marks only on 5 second intervals (0, 5, 10, ...)
//		//		--------------------------------------------------------
//
//		if (!(i % 5)) {
//
//			cairo_set_source_rgb(cr, 0.5, 0.5, 1.0); // blue-ish
//
//			//------------------------------------------------------------------------
//			//			Get coordinates of outer end of long hash mark
//			//------------------------------------------------------------------------
//
//			cairo_arc(cr, hor, ver, meter_radius + 10, -M_PI, -M_PI + X); //long length hash
//			cairo_get_current_point(cr, &x, &y); // sets x and y
//
//			//-----------------------------------------------------------------
//			//			Get coordinates of inner end of hash mark
//			//-----------------------------------------------------------------
//
//			cairo_arc(cr, hor, ver, meter_radius, -M_PI, -M_PI + X); // short length hash
//			cairo_get_current_point(cr, &x1, &y1); // sets x1 and y1
//
//			//----------------------------------------------------------------
//			//			Draw hash mark from (x,y) to (x1,y1)
//			//----------------------------------------------------------------
//
//			cairo_new_path(cr);
//			cairo_move_to(cr, x, y);
//			cairo_line_to(cr, x1, y1);
//
//			cairo_stroke(cr);
//		}
//
//		else if (cFlg == 1) { // seconds
//
//			if (i % 5) {
//
//				cairo_set_source_rgb(cr, 1.0, 1.0, 0.3); // yellow
//
//				cairo_arc(cr, hor, ver, needle_length + 2, -M_PI, -M_PI + X); //long length
//				cairo_get_current_point(cr, &x, &y);
//
//				cairo_arc(cr, hor, ver, needle_length - 1, -M_PI, -M_PI + X); // short length
//				cairo_get_current_point(cr, &x1, &y1);
//
//				if (X > needle_position) break;
//
//				cairo_new_path(cr);
//				cairo_move_to(cr, x, y);
//
//				cairo_line_to(cr, x1, y1);
//
//				cairo_stroke(cr);
//			}
//		}
//		//----------------------------------------------------------------------------
//		//		There are 30 hash marks per half circle (60 for full circle)
//		//		A half circle is M_PI. A full circle is 2 * M_PI so each
//		//		hash mark is M_PI / 30.0 apart.
//		//----------------------------------------------------------------------------
//
//		X = X + M_PI / 30.0;
//	}
//
//	//------------------
//	//	Pick a color
//	//------------------
//
//	if (cFlg == 1) cairo_set_source_rgb(cr, 1.0, 1.0, 0.3); // yellow
//	else if (cFlg == 2) cairo_set_source_rgb(cr, 1.0, 0., 0.); // red
//	else if (cFlg == 3) cairo_set_source_rgb(cr, 0.5, 1.0, 0.5); // green
//
//	//----------------
//	//	needle
//	//----------------
//
//	if (cFlg == 1)  cairo_set_line_width(cr, 1.0);
//	else if (cFlg == 2)  cairo_set_line_width(cr, 2.0);
//	else cairo_set_line_width(cr, 3.0);
//
//	cairo_arc(cr, hor, ver, needle_length, -M_PI, -M_PI + needle_position);
//	cairo_get_current_point(cr, &x, &y);
//	cairo_new_path(cr);
//	cairo_move_to(cr, x, y);
//	cairo_line_to(cr, hor, ver);
//	cairo_stroke(cr);
//
//	//----------------
//	//	spindle
//	//----------------
//
//	cairo_arc(cr, hor, ver, 4.0, -M_PI, M_PI); // -M_PI -> 0.0 -> +M_PI : full circle
//	cairo_line_to(cr, hor, ver + 3);
//	cairo_fill(cr); // fill in arc
//
//}
//
//gboolean on_draw2_draw(GtkDrawingArea* widget, cairo_t* cr) {
//
//	double hor, ver, needle_length, meter_radius, needle_position;
//
//#define MHOR 106.0
//#define MVER 76.0
//
//	hor = MHOR;
//	ver = MVER;
//	meter_radius = 50;
//
//
//	needle_position = hour + 3.0 + minute / 60.0;
//
//	//--------------------------------------------------------------------------
//	//	6 hours per half circle so hour marks are hour * (M_PI / 6.0) apart
//	//--------------------------------------------------------------------------
//
//	needle_position = needle_position * (M_PI / 6.0);
//	needle_length = 30.0;
//	needles(cr, hor, ver, needle_length, needle_position, meter_radius, 3);
//
//	//--------------------------------
//	//	minute zero adjustment
//	//--------------------------------
//
//	needle_position = minute + 15;
//
//	//----------------------------------------------
//	//	Minutes are 30 per half circle apart
//	//----------------------------------------------
//
//	needle_position = needle_position * (M_PI / 30.0);
//	needle_length = 45.0;
//	needles(cr, hor, ver, needle_length, needle_position, meter_radius, 2);
//
//	//----------------------------------------------------------
//	//	Seconds handles similar to minutes (60 per circle)
//	//----------------------------------------------------------
//
//	needle_position = second + 15;	// adjust zero to top of dial
//	needle_position = needle_position * (M_PI / 30.0);
//	needle_length = 50.0;
//	needles(cr, hor, ver, needle_length, needle_position, meter_radius, 1);
//
//	//---------------------------------
//	//	Show digital text of time
//	//---------------------------------
//
//	cairo_move_to(cr, 40.0, 150.0); // location: horizontal, vertical
//	cairo_show_text(cr, time_string); // you can change the font if you want
//	cairo_stroke(cr);
//
//	return FALSE;
//}
//
//
//#include <signal.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <gtk/gtk.h>
//#include <math.h>
//#include <stdlib.h>
//#include <stdbool.h>
//#include <string.h>
//#include <errno.h>
//#include <time.h>
//#include <ctype.h>
//
//GtkWidget* window;
//GtkWidget* draw2;
//
//gboolean	on_draw2_draw(GtkDrawingArea* widget, cairo_t* cr);
//char		bg_color[256] = "black";	// background color
//
//void activate(GtkApplication* app, gpointer user_data)
//{
//	GtkWidget* window;
//	GtkButton* button;
//	GtkWidget** area;
//
//	window = gtk_application_window_new(app);
//	gtk_window_set_title(GTK_WINDOW(window), "Complex numbers");
//	gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
//	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
//
//	GtkBox* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
//	gtk_window_set_child(GTK_WINDOW(window), box);
//
//	draw2 = gtk_drawing_area_new();
//	gtk_widget_set_size_request(area, WIDTH, HEIGHT);
//	gtk_box_append(box, area);
//
//	button = gtk_button_new_with_label("START");
//	gtk_widget_set_size_request(button, 200, 200);
//	gtk_box_append(box, button);
//
//
//	g_signal_connect(button, "button-press-event", G_CALLBACK(draw_graph), NULL);
//
//
//	gtk_widget_show(window);
//	gtk_window_present(GTK_WINDOW(window));
//
//	g_timeout_add_seconds(1, (GSourceFunc)clock_timer_handler, NULL);
//
//
//}
//
//void complex_numbers_dbg()
//{
//	GtkApplication* app;
//	int status;
//
//	app = gtk_application_new("org.gtk", G_APPLICATION_FLAGS_NONE);
//	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
//	status = g_application_run(G_APPLICATION(app), 0, NULL);
//	g_object_unref(app);
//
//}
