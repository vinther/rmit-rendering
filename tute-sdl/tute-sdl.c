/* $Id: example.c 4 2006-07-03 12:40:56Z aholkner $ */

#include "sdl-base.h"

#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

static int yaw;

static int width = 1024;
static int height = 768;

static float text_y = 32.0f;
static float text_x = 32.0f;

/* Called once at the beginning of the program, after SDL is initialised. */
void init()
{
  int argc = 0;  
  char *argv = "";
  glutInit(&argc, &argv);

  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_FLAT);

  yaw = 0;
}

/* Called every time the window is resized. */
void reshape(int w, int h)
{
  width = w; height = h;

  glViewport (0, 0, (GLsizei) width, (GLsizei) height); 
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(65.0, (GLfloat) width/(GLfloat) height, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef (0.0, 0.0, -5.0);
}

/* Called once every frame. */
void display(SDL_Surface *screen)
{
  const char *c;
  const size_t buffer_size = 128;
  char string[buffer_size];

  glClearColor(1, 0, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT);

  glPushMatrix();
  glRotatef(yaw, 0.0f, 1.0f, 0.0f);

  glutSolidTeapot(1.0f);

  glPopMatrix();

  /* Drawing text */
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glLoadIdentity();
  glOrtho(0, 512, 0, 512, 0, 1000);

  glMatrixMode(GL_MODELVIEW);

  snprintf(string, buffer_size, "%5d FPS, %d yaw\r", fps, yaw);

  glRasterPos2f(text_x, text_y);
  for (c = string; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
  }

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  /* Back to normal drawing */

  glMatrixMode(GL_MODELVIEW);

  fflush(stdout);
}

/* Called every frame, with the number of milliseconds since the last frame. */
void update(int milliseconds)
{
}

/* Called every time an event needs to be processed. */
void event(SDL_Event *event)
{
  if (SDL_KEYDOWN == event->type)
  {
    switch (event->key.keysym.sym)
    {
      case (SDLK_ESCAPE):
        exit(EXIT_SUCCESS);
        break;
      case (SDLK_LEFT):
        yaw = (yaw - 10) % 360;
        break;
      case (SDLK_RIGHT):
        yaw = (yaw + 10) % 360;
        break;
      default:
        break;
    }
  }
}

/* Called before quit */
void cleanup()
{
}
