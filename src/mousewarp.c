/* Copyright (C) 2018 Renato Alves <alves.rjc@gmail.com>
 */

/*
 * This software displays 4 colored quadrants on screen to help
 * mouse warping when using Keyboardio's MouseWarp keys
 *
 * 4 quadrants are displayed, numbered:
 *
 *   +-----+-----+                   +-----+--+--+
 *   |     |     |                   |     | 1| 2|
 *   |  1  |  2  |                   |     +--+--+
 *   |     |     |                   |     | 3| 4|
 *   +-----+-----+  -> STDIN '2' ->  +-----+--+--+  -> ...
 *   |     |     |                   |     |     |
 *   |  3  |  4  |                   |     |     |
 *   |     |     |                   |     |     |
 *   +-----+-----+                   +-----+-----+
 *
 * This app reads from STDIN in order to know which quadrant to further split
 * in order to narrow down the region to warp into.
 *
 * 6 possible values are understood when given through STDIN:
 *  0 -> reset quadrant zooming, aka use full screen 
 *  1 to 4 -> warp to given quadrant
 *  EOF -> terminate warping
 */
 
#include <cairo-xlib.h>
#include <aosd.h>

#include <stdio.h>

#define DEBUG 1
#define debug_print(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

typedef struct {
  cairo_surface_t* surf;
  int width;    // screen width
  int height;   // screen height
  int top_x;    // top left corner
  int top_y;    // top left corner
  int q_width;  // width of quadrant
  int q_height; // height of quadrant
  int mid_x;    // center of all quadrants
  int mid_y;    // center of all quadrants
  float alpha;  // how much fake transparency to use
} RenderData;

static void render_quadrants(cairo_t* cr, void* data)
{
  RenderData* d = data;

  // top-left quadrant - pastel red
  cairo_set_source_rgba(cr, 1, 0.702, 0.729, d->alpha);
  cairo_rectangle(cr, d->top_x, d->top_y, d->q_width, d->q_height);
  debug_print("top left %d %d %d %d\n", d->top_x, d->top_y, d->q_width, d->q_height);
  cairo_fill(cr);

  // top-right quadrant - pastel yellow
  cairo_set_source_rgba(cr, 1, 1, 0.729, d->alpha);
  cairo_rectangle(cr, d->mid_x, d->top_y, d->q_width, d->q_height);
  debug_print("top right %d %d %d %d\n", d->mid_x, d->top_y, d->q_width, d->q_height);
  cairo_fill(cr);

  // bottom-left quadrant - pastel green
  cairo_set_source_rgba(cr, 0.729, 1, 0.788, d->alpha);
  cairo_rectangle(cr, d->top_x, d->mid_y, d->q_width, d->q_height);
  debug_print("bottom left %d %d %d %d\n", d->top_x, d->mid_y, d->q_width, d->q_height);
  cairo_fill(cr);

  // bottom-right quadrant - pastel blue
  cairo_set_source_rgba(cr, 0.729, 0.882, 1, d->alpha);
  cairo_rectangle(cr, d->mid_x, d->mid_y, d->q_width, d->q_height);
  debug_print("bottom right %d %d %d %d\n", d->mid_x, d->mid_y, d->q_width, d->q_height);
  cairo_fill(cr);

  cairo_set_source_surface(cr, d->surf, 0, 0);
  cairo_paint(cr);
}

static void update_mid_points(RenderData* d) {
  d->mid_x = d->top_x + d->q_width;
  d->mid_y = d->top_y + d->q_height;

  debug_print("%d (%d) %d - %d (%d) %d\n",
      d->top_x, d->mid_x, d->q_width,
      d->top_y, d->mid_y, d->q_height);

}

static void update_quadrants(RenderData* d, int quadrant)
{
  switch (quadrant) {
    case 0 :
      // Reset to full screen
      d->top_x = 0;
      d->top_y = 0;
      d->q_width = d->width;
      d->q_height = d->height;
      break;
    case 1 :
      // needs only to update how big are the quadrants
      break;
    case 2 :
      // top-right quadrant
      d->top_x = d->mid_x;
      break;
    case 3 :
      // bottom-left quadrant
      d->top_y = d->mid_y;
      break;
    case 4 :
      // bottom-right quadrant
      d->top_x = d->mid_x;
      d->top_y = d->mid_y;
      break;
  }

  // After each update we shrink each quadrant to half the size
  d->q_width = d->q_width / 2;
  d->q_height = d->q_height / 2;

  // And recompute the middle points
  update_mid_points(d);
}

int main(int argc, char* argv[])
{
  // X11 structures
  Display* dsp;
  Screen* screen;

  // Aosd structures
  Aosd* aosd;
  RenderData data = {.alpha = 0.3};

  dsp = XOpenDisplay(NULL);
  // Get width/height of display where mouse is
  screen = XScreenOfDisplay(dsp, DefaultScreen(dsp));

  data.width = XWidthOfScreen(screen);
  data.height = XHeightOfScreen(screen);

  printf("Screen coordinates: H:%d W:%d\n",
      data.height, data.width);

  data.surf = cairo_xlib_surface_create(
      dsp, 0, 0, data.height, data.width);

  // Reset to full screen quadrants
  update_quadrants(&data, 0);

  aosd = aosd_new();
  aosd_set_transparency(aosd, TRANSPARENCY_COMPOSITE);
  aosd_set_hide_upon_mouse_event(aosd, True);

  // This sets the area where aosd is allowed to draw
  // If full width and height, the pseudo transparency
  // causes the screen to appear frozen until an action is taken
  aosd_set_geometry(aosd, data.top_x, data.top_y, data.width, data.height);
  aosd_set_renderer(aosd, render_quadrants, &data);

  aosd_show(aosd);

  // FIXME the do...while isn't necessary as long as we don't destroy the surface/aosd
  do
  {
    aosd_render(aosd);  // Call this when a redraw is necessary
    aosd_loop_for(aosd, 1000);
    // TODO Read new quadrant coordinates from STDIN a'la' Focus plugin in
    // Keyboardio and update the quadrant coordinates to use for next render
    update_quadrants(&data, 4);
  } while (aosd_get_is_shown(aosd));

  cairo_surface_destroy(data.surf);
  aosd_destroy(aosd);
  XCloseDisplay(dsp);

  return 0;
}

/* vim: set ts=2 sw=2 et : */
