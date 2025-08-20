#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "bmptype.h"

// math symbol
#define PI  3.141592653

// windoe size
int width = 600, height = 600;

// define GLU object ex.sphere, cylind......
GLUquadricObj* sphere = NULL, * cylind = NULL;

// vertices of cube
float points[][3] = {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
                     {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
                     {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
                     {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}};

// 6 face of cube, have 8 vertices
int face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6 ,5}, {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};

int cube[6] = {0, 1, 2, 3, 4, 5};
// save the position of object
float position[3] = {8.0, 0.0, 0.0};
float position1[4][1] = {8.0, 0.0, 0.0, 1.0};
// world space
float dx = 0.0, dy = 0.0, dz = 0.0;
// eye position
float   eye[3], Eye[3];
float   eye1[4][1] = {0.0, 0.0, 0.0, 1.0};
float   im[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
float   eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float cos5, sin5;
// angle of helicopter  global angle    tilt angle      carmera angle
float self_ang = 0.0, glob_ang = 0.0, tilt_ang = 0.0, carmera_ang = 90.0, carmerax_ang = 40.0;

// H-arm angle         blades angle
float  arm_ang = 45.0, bld_ang = 10.0;

// check fly or not
int up = 0, down = 0;

// rotation matrix
float Rx[4][4] = {{1.0, 0.0, 0.0, 0.0},
                  {0.0, cos(self_ang), -sin(self_ang), 0.0},
                  {0.0, sin(self_ang), cos(self_ang), 0.0},
                  {0.0, 0.0, 0.0, 1.0}};

float Ry[4][4] = {{cos(self_ang), 1.0, sin(self_ang), 0.0},
                  {0.0, 1.0, 0.0, 0.0},
                  {-sin(self_ang), 0.0, cos(self_ang), 0.0},
                  {0.0, 0.0, 0.0, 1.0}};

float Rz[4][4] = {{cos(self_ang), -sin(self_ang), 0.0, 0.0},
                  {sin(self_ang), cos(self_ang), 0.0, 0.0},
                  {0.0, 0.0, 1.0, 0.0},
                  {0.0, 0.0, 0.0, 1.0}};

// 0: 4 window, 1: x-direction, 2: y-direction 3: z-direction, 4: perspective projection
int mode = 0;

// define spot light propertices
float spot_position[] = {4.0, 0.0, 6.0, 1.0};
float spot_direction[] = {1.0, 0.0, 0.0, 0.0};
float spot_cutoff = 45.0;
float spot_exponent = 8.0;
float spot_diffuse[] = {1.0,1.0,1.0, 1.0};
float spot_specular[] = {1.0,1.0,1.0, 1.0};
// define directional light propertices
float sun_direction[] = {-1.0, -1.0, 0.0, 0.0};
float sun_diffuse[] = {0.7, 0.7, 0.7, 1.0};
// define point light propertices
float point_position[] = {5, 15, 0.0, 1.0};
float point_diffuse[] = {0.7, 0.7, 0.7, 1.0};
float point_specular[] = {0.9, 0.9, 0.9, 1.0};
float point_emission[] = {0.1, 0.1, 0.1, 1.0};

float global_ambient[] = {0.2, 0.2, 0.2, 1.0};

// define helicopter's metarial (metallic)
float met_ambient[] = {0.1, 0.3, 0.1, 1.0};
float met_diffuse[] = {0.1, 0.9, 0.1, 1.0};
float met_specular[] = {0.4, 0.4, 0.4, 1.0};
float met_shininess = 64.0;
// define floor's metarial (diffuse)
float flr_ambient[] = {0.3, 0.3, 0.3, 0.0};
float flr_diffuse[] = {0.6, 0.6, 0.6, 1.0};
float flr_specular[] = {0.0, 0.0, 0.0, 1.0};

// turn on/off light
bool sun_switch = true;
bool point_switch = true;
bool spot_switch = true;

float point_angle = 0.0;

// switch color
int sun_color = 0;
int point_color = 0;
int spot_color = 0;

// intensity of lights
float sun_intensity = 0.1;
float point_intensity = 0.1;
float spot_intensity = 0.1;

// emission
float emission = 1.0f;

// texture
#define TSIZE 64
// declare three texture map
unsigned int TextureID[8];
int select = 0;
unsigned char star[TSIZE][TSIZE][4];
unsigned char tree[TSIZE][TSIZE][4];

// billboard
float matrix[16];
float a[3], b[3], c[3];

// fog
bool fog_switch = true;
int fog_mode = 0; // linear, exp, exp2
float fog_color[] = {0.15, 0.20, 0.20, 0.50};
float fog_density = 0.1;

// call from myinit(): create texture map
void make_star() {
    int x, y, i;

    for (x = 0; x < 64; x++)
        for (y = 0; y < 64; y++) {
            star[x][y][0] = 255;
            star[x][y][1] = 255;
            star[x][y][2] = 255;
            star[x][y][3] = 255;
        }

    x = 31;
    y = 60;
    while (x >= 12) {
        for (i = y; i > 4; i--) {
            star[x][i][0] = 100;
            star[x][i][1] = 100;
            star[x][i][2] = 255;
            star[x][i][3] = 255;
        }
        y -= 3;
        x--;
    }
    x = 31;
    y = 20;
    while (x >= 12) {
        for (i = y; i > 4; i--) {
            star[x][i][0] = 255;
            star[x][i][1] = 255;
            star[x][i][2] = 255;
            star[x][i][3] = 255;
        }
        y -= 1;
        x--;
    }
    x = 4;
    y = 40;
    while (y >= 18) {
        for (i = x; i < 32; i++) {
            star[i][y][0] = 100;
            star[i][y][1] = 100;
            star[i][y][2] = 255;
            star[i][y][3] = 255;
        }
        y -= 1;
        if ((y % 2) == 1)
            x += 2;
        else
            x++;
    }

    for (x = 63; x > 31; x--)
        for (y = 0; y < 64; y++)
            for (i = 0; i < 4; i++)
                star[x][y][i] = star[63 - x][y][i];
}
// call from myinit()
void make_tree() {
    int i, j, c;
    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++) {
            if (j > i / 2 && j <= 64 - i / 2)
                c = 255;
            else
                c = 0;
            tree[i][j][0] = c / 8;
            tree[i][j][1] = c / 2;
            tree[i][j][2] = c / 4;
            if (c == 255)
                tree[i][j][3] = 255;
            else
                tree[i][j][3] = 0;
        }
    // Generate trunk
    for (i = 0; i < TSIZE / 3; i++) {
        for (j = 0; j < TSIZE / 2 - 4; j++)
            tree[i][j][3] = 0;
        for (j = TSIZE / 2 + 4; j < TSIZE; j++)
            tree[i][j][3] = 0;
    }
}
// call from myinit()
void make_sky() { // load bmp file and change to texture 
    glClearColor(0.0, 0.0, 0.0, 0.5);
    BMP info1 = BMP("C:/2023 code/Helicopter4/image/Daylight Box_Front.bmp");
    BMP info2 = BMP("C:/2023 code/Helicopter4/image/Daylight Box_Back.bmp");
    BMP info3 = BMP("C:/2023 code/Helicopter4/image/Daylight Box_Top.bmp");
    BMP info4 = BMP("C:/2023 code/Helicopter4/image/Daylight Box_Bottom.bmp");
    BMP info5 = BMP("C:/2023 code/Helicopter4/image/Daylight Box_Left.bmp");
    BMP info6 = BMP("C:/2023 code/Helicopter4/image/Daylight Box_Right.bmp");

    glGenTextures(1, &TextureID[1]); // create texture 1
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info1.GetWidth(), info1.GetWidth(), 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, info1.GetPixels().data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &TextureID[2]); // create texture 2
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info2.GetWidth(), info2.GetWidth(), 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, info2.GetPixels().data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &TextureID[3]); // create texture 3
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info3.GetWidth(), info3.GetWidth(), 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, info3.GetPixels().data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &TextureID[4]); // create texture 4
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info4.GetWidth(), info4.GetWidth(), 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, info4.GetPixels().data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &TextureID[5]); // create texture 5
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[5]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info5.GetWidth(), info5.GetWidth(), 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, info5.GetPixels().data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &TextureID[6]); // create texture 6
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[6]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info6.GetWidth(), info6.GetWidth(), 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, info6.GetPixels().data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
}

// call from main() : initialize environment
void myinit(){
    // set the background clolr to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    // clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glShadeModel(GL_SMOOTH);
    // enable depth buffer for shading computing
    glEnable(GL_DEPTH_TEST);
    // to normalize normals after conducting geometrical transformation
    glEnable(GL_NORMALIZE);
    // enable lighting effects
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    // define reflection degree
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    // define global ambient light
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    // not to draw the face can't see  
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // texture map
    glEnable(GL_TEXTURE_2D);
    glGenTextures(8, TextureID);

    // define texture 1 (star)
    make_star();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // define filtering methods for magnification and minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // load image data
    glTexImage2D(GL_TEXTURE_2D,
        0, // 1 level texture(no mipmap)
        GL_RGBA, // texel format
        TSIZE, TSIZE, // size of image data
        0, // width of texture boarder
        GL_RGBA, // pixel format
        GL_UNSIGNED_BYTE, // data type of pixel component
        star);

    // define texture 2 (sky box)
    make_sky();

    // define billboard
    make_tree();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[7]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // define filtering methods for magnification and minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // load image data
    glTexImage2D(GL_TEXTURE_2D,
        0, // 1 level texture(no mipmap)
        GL_RGBA, // texel format
        TSIZE, TSIZE, // size of image data
        0, // width of texture boarder
        GL_RGBA, // pixel format
        GL_UNSIGNED_BYTE, // data type of pixel component
        tree);

    // define fog
    glEnable(GL_FOG);                 // enable fog fade
    glFogi(GL_FOG_MODE, GL_LINEAR);   // fog factor = GL_LINEAR, GL_EXP, GL_EXP2
    glFogf(GL_FOG_DENSITY, fog_density);     // fog opacity(density)= 0.1
    glFogf(GL_FOG_START, 1.0);
    glFogf(GL_FOG_END, 36.0);
    glFogfv(GL_FOG_COLOR, fog_color); // set fog color

    // enforce window system display the result
    glFlush();

    // copy eye position
    eye[0] = Eye[0];
    eye[1] = Eye[1];
    eye[2] = Eye[2];
}
// call from display_func()
void draw_view() {
    
    glMatrixMode(GL_MODELVIEW);
    // draw eye position : green
    glPushMatrix(); // E1 push
    glTranslatef(eye[0], eye[1], eye[2]);
    glTranslatef(eye1[1][0], eye1[2][0], eye1[3][0]);
    glColor3f(0.0, 1.0, 0.0);
    glutWireSphere(1.0, 10, 10);
    glPopMatrix(); // E1 pop

    // draw eye coord. Xe axes : yellow
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * im[0][0], eye[1] + 20.0 * im[0][1], eye[2] + 20.0 * im[0][2]);
    glEnd();
    // draw eye coord. Ye axes : pink
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * im[1][0], eye[1] + 20.0 * im[1][1], eye[2] + 20.0 * im[1][2]);
    glEnd();
    // draw eye coord. Ze zxes : cyan
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * im[2][0], eye[1] + 20.0 * im[2][1], eye[2] + 20.0 * im[2][2]);
    glEnd();

    // draw view volume
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
    glPushMatrix(); // E2 push
    glTranslatef(eye[0], eye[1], eye[2]);
    glRotatef(eyeAngx, -1, 0, 0);
    glRotatef(eyeAngy, 0, 1, 0);
    glRotatef(eyeAngz, 0, 0, 1);

    // eye to near clipping plane
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 1.5, eye[1] + 1.5, eye[2] - 1.5);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0]- 1.5, eye[1] + 1.5,eye[2] - 1.5);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] - 1.5, eye[1] - 1.5, eye[2] - 1.5);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 1.5, eye[1] + 1.5, eye[2] - 1.5);
    glEnd();

    // far plane color is gray
    glColor3f(0.75, 0.75, 0.75);
    glBegin(GL_LINES);
    glVertex3f(eye[0] + 50, eye[1] + 50, eye[2] - 50);
    glVertex3f(eye[0] - 50, eye[1] + 50, eye[2] - 50);
    glVertex3f(eye[0] - 50, eye[1] - 50, eye[2] - 50);
    glVertex3f(eye[0] + 50, eye[1] - 50, eye[2] - 50);
    glEnd();
    
    // near plane color is light gray
    glColor3f(0.82, 0.82, 0.82);
    glBegin(GL_LINES);
    glVertex3f(eye[0] + 1.5, eye[1] + 1.5, eye[2] - 1.5);
    glVertex3f(eye[0] - 1.5, eye[1] + 1.5, eye[2] - 1.5);
    glVertex3f(eye[0] - 1.5, eye[1] - 1.5, eye[2] - 1.5);
    glVertex3f(eye[0] + 1.5, eye[1] - 1.5, eye[2] - 1.5);
    glEnd();

    // line color is LightSlateGray
    glColor3f(0.46, 0.53, 0.60);
    glBegin(GL_LINES);
    glVertex3f(eye[0] - 50, eye[1] + 50, eye[2] - 50);
    glVertex3f(eye[0] - 1.5, eye[1] + 1.5, eye[2] - 1.5);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(eye[0] + 50, eye[1] + 50, eye[2] - 50);
    glVertex3f(eye[0] + 1.5, eye[1] + 1.5, eye[2] - 1.5);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(eye[0] + 50, eye[1] - 50, eye[2] - 50);
    glVertex3f(eye[0] + 1.5, eye[1] - 1.5, eye[2] - 1.5);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(eye[0] - 50, eye[1] - 50, eye[2] - 50);
    glVertex3f(eye[0] - 1.5, eye[1] - 1.5, eye[2] - 1.5);
    glEnd();

    glPopMatrix(); // E2 pop
}
// call from draw_helicopter
void draw_axes()
{
    // draw a sphere be the origin
    glColor3f(0.9, 0.9, 0.9);
    if (sphere == NULL) {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
    }
    gluSphere(sphere, 1.0,
        12,
        12);
    // draw z axis in world space : cyan
    glColor3f(0.0, 0.95, 0.95);
    if (cylind == NULL) {
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    gluCylinder(cylind, 0.5, 0.5,
        10.0,
        12,
        3);
    // glPopMatrix();
    // draw y axis in world space : pink
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0); // according x axis to rotate z -90 be y
    glColor3f(0.95, 0.0, 0.95);
    gluCylinder(cylind, 0.5, 0.5,
        10.0,
        12,
        3);
    glPopMatrix();

    // draw x axis in world space: yellow
    glPushMatrix();
    glColor3f(0.95, 0.95, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);// according z axis to rotate y 90 be z
    gluCylinder(cylind, 0.5, 0.5,
        10.0,
        12,
        3);
    glPopMatrix();
}

// call from draw_helicopter()
void draw_background(int index) {
    // rotate light position
    glPushMatrix();//M0
    glTranslatef(8.0, 0.0, 10.0);
    glRotatef(point_angle, 0.0, 1.0, 0.0);

    // Draw point light as a red sphere
    // set material (metallic)
    glMaterialfv(GL_FRONT, GL_AMBIENT, global_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, point_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, point_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, point_emission);
    glNormal3f(0.0, 0.0, 1.0);

    glPushMatrix();//M1
    glTranslatef(point_position[0], point_position[1], point_position[2]);
    // glColor3f(1.0, 0.0, 0.0);

    glutSolidSphere(0.5, 12, 12);
    glPopMatrix();//M1

    // redefine position and direction of light
    glLightfv(GL_LIGHT2, GL_POSITION, point_position);
    glPopMatrix();//M0

    // define floor's material (no specular)
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, flr_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, flr_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, flr_ambient);
    glNormal3f(0.0, 1.0, 0.0);

    // set texture function
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[index]);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            // if (i == 4 || i == 6) glColor3f(1.0, 1.0, 0.0); // grass 
            // else if (i < 2 || i > 8)glColor3f(0.48,0.98,0); // yellow line
            // else glColor3f(0.2, 0.2, 0.4); // road
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);            
            glVertex3f((i - 5.0) * 10.0, -2.2, (j - 5.0) * 10.0);
            glTexCoord2f(0.0, 8.0);
            glVertex3f((i - 5.0) * 10.0, -2.2, (j - 4.0) * 10.0);
            glTexCoord2f(8.0, 8.0);
            glVertex3f((i - 4.0) * 10.0, -2.2, (j - 4.0) * 10.0);
            glTexCoord2f(8.0, 0.0);
            glVertex3f((i - 4.0) * 10.0, -2.2, (j - 5.0) * 10.0);
            glEnd();
        }
    }
}
// call from draw_helicopter
void draw_sky() {
    glPushMatrix();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[1]);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    // front
    glPushMatrix();
    glBegin(GL_POLYGON);
    glTexCoord2d(1, 0);
    glVertex3f(  50.0, -50.0, -50.0);
    glTexCoord2d(1, 1);
    glVertex3f(  50.0, 50.0, -50.0);
    glTexCoord2d(0, 1);
    glVertex3f( -50.0, 50.0, -50.0);
    glTexCoord2d(0, 0);
    glVertex3f( -50.0, -50.0, -50.0);
    glEnd();
    glPopMatrix();
    // left
    glBindTexture(GL_TEXTURE_2D, TextureID[5]);
    glBegin(GL_POLYGON);
    glTexCoord2d(1, 0);
    glVertex3f( -50.0, -50.0, -50.0);
    glTexCoord2d(1, 1);
    glVertex3f( -50.0, 50.0, -50.0);
    glTexCoord2d(0, 1);
    glVertex3f( -50.0, 50.0,  50.0);
    glTexCoord2d(0, 0);
    glVertex3f( -50.0, -50.0,  50.0);
    glEnd();
    // back
    glBindTexture(GL_TEXTURE_2D, TextureID[2]);
    glBegin(GL_POLYGON);
    glTexCoord2d(1, 0);
    glVertex3f( -50.0, -50.0, 50.0);
    glTexCoord2d(1, 1);
    glVertex3f( -50.0, 50.0, 50.0);
    glTexCoord2d(0, 1);
    glVertex3f(  50.0, 50.0, 50.0);
    glTexCoord2d(0, 0);
    glVertex3f(  50.0, -50.0, 50.0);
    glEnd();
    // right
    glBindTexture(GL_TEXTURE_2D, TextureID[6]);
    glBegin(GL_POLYGON);
    glTexCoord2d(1, 0);
    glVertex3f( 50.0, -50.0,  50.0);
    glTexCoord2d(1, 1);
    glVertex3f( 50.0, 50.0,  50.0);
    glTexCoord2d(0, 1);
    glVertex3f( 50.0, 50.0, -50.0);
    glTexCoord2d(0, 0);
    glVertex3f( 50.0, -50.0, -50.0);
    glEnd();
    // top
    glBindTexture(GL_TEXTURE_2D, TextureID[3]);
    glBegin(GL_POLYGON);
    glTexCoord2d(0, 1);
    glVertex3f( -50.0, 50.0, 50.0);
    glTexCoord2d(0, 0);
    glVertex3f( -50.0, 50.0, -50.0);
    glTexCoord2d(1, 0);
    glVertex3f(  50.0, 50.0, -50.0);
    glTexCoord2d(1, 1);
    glVertex3f(  50.0, 50.0,  50.0);
    glEnd();
    // bottom
    // glBindTexture(GL_TEXTURE_2D, TextureID[4]);
    // glBegin(GL_POLYGON);
    // glNormal3f(0.0, 0.0, 1.0);
    // glTexCoord2d(0, 0);
    // glVertex3f( -25.0,  0.0,  50.0);
    // glTexCoord2d(0, 1);
    // glVertex3f( -25.0,  0.0, -50.0);
    // glTexCoord2d(1, 1);
    // glVertex3f(  25.0,  0.0, -50.0);
    // glTexCoord2d(1, 0);
    // glVertex3f(  25.0,  0.0,  50.0);
    // glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
}
// call from myinit()
void draw_billboard(float x, float z, float w, float h) {
    float v0[3], v1[3], v2[3], v3[3];
    // set texture   
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TextureID[7]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // enable testing
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBegin(GL_QUADS);
    // v0
    glTexCoord2f(0.0, 0.0);
    glVertex3f(x - w/2 * a[0], 0.0, z - w/2 * a[2]);
    // v1
    glTexCoord2f(1.0, 0.0);
    glVertex3f(x + w/2 * a[0], 0.0, z + w/2 * a[2]);
    // v2
    glTexCoord2f(1.0, 1.0);
    glVertex3f(x + w/2 * a[0], h, z + w/2 * a[2]);
    // v3
    glTexCoord2f(0.0, 1.0);
    glVertex3f(x - w/2 * a[0], h, z - w/2 * a[2]);
    glEnd();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

// call from draw_helicopter()
void draw_blade(){
    // draw 4 retangle
    glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 4.0, 0.0);
	glVertex3f(1.0, 8.0, 0.0);
	glVertex3f(-1.0, 8.0, 0.0);
	glVertex3f(-1.0, 4.0, 0.0);
	glEnd();
}
//  call from draw_helicopter()
void draw_cube(){
    // set material (metallic)
    glMaterialfv(GL_FRONT, GL_AMBIENT, met_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, met_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, met_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, met_shininess);
    // glColor3f(0.08, 0.20, 0.0);
    for(int i = 0; i < 6; i++) {
        glBegin(GL_POLYGON);
        glVertex3fv(points[face[i][0]]);
        glVertex3fv(points[face[i][1]]);
        glVertex3fv(points[face[i][2]]);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
}

// call from main, display, idle_func, reshape_func, keyboard_func
void draw_helicopter() {

    glClearColor(0.0, 0.37, 1.0, 0.0);

    glPushMatrix();
    glTranslatef(position[0], position[1], position[2]);
    glTranslatef(position1[0][0], position1[1][0], position1[2][0]);
    glRotatef(self_ang, 0.0, 1.0, 0.0);
    
    glLightfv(GL_LIGHT0, GL_POSITION, spot_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glPopMatrix();

    // draw the background
    draw_background(select);

    // draw billboard
    // glMaterialfv(GL_FRONT, GL_DIFFUSE, flr_diffuse);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[7]);
    draw_billboard(5.0 * 4.0, 3.0 * 4.0, 5.0, 8.0);
    draw_billboard(6.0 * 4.0, 5.0 * 4.0, 5.0, 8.0);
    draw_billboard(3.0 * 4.0, 6.0 * 4.0, 5.0, 8.0);
    draw_billboard(2.0 * 4.0, 7.0 * 4.0, 5.0, 8.0);
    draw_billboard(7.0 * 4.0, 2.0 * 4.0, 5.0, 8.0);
    glDisable(GL_TEXTURE_2D);

    // draw_sky
    draw_sky();

    // draw xyz axis and eye axis 
    draw_axes();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPushMatrix();                                                       //M1 push
    glTranslatef(position[0], position[1], position[2]);
    glTranslatef(position1[0][0], position1[1][0], position1[2][0]);
    // printf("%f\n", tilt_ang);
    glRotatef(self_ang, 0.0, 1.0, 0.0);
    glRotatef(tilt_ang, 1.0, 0.0, 0.0);
    
    glPushMatrix();                                                       //M2 push
    glScalef(8.0, 4.0, 8.0); 
    glTranslatef(0, 1, 0);
    // spot light position = 直升機的position
    spot_position[0] = position[0], spot_position[1] = position[1], spot_position[2] = position[2];
    // draw the body of helicopter
    draw_cube();
    glPopMatrix();                                                        //M2 pop

    // draw the head of helicopter
    glColor3f(0.08, 0.20, 0.0);
    glPushMatrix();                                                       //MDE1 push
    glTranslatef(0.0, 10.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glPushMatrix();                                                       //M3 push
    if (cylind == NULL) {
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    gluCylinder(cylind, 0.5, 0.5,
        4.0,
        12,
        3);
    glPopMatrix();                                                       //M3 pop

    // draw top ball in the middle of blade
    glPushMatrix();                                                      //M4 push
    if (sphere == NULL) {
	    sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
	}
	glColor3f(0.06, 0.06, 0.06);
	gluSphere(sphere, 0.5, // radius
		12,            // 12 slices
		12);           // 12 stack
	glRotatef(arm_ang, 1.0, 0.0, 0.0);
	glPopMatrix();                                                         //M4 pop

    // draw  three blades
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();                                                        //M5 push
	glRotatef(bld_ang, 0.0, 0.0, 1.0);
	draw_blade(); // draw the first blade

	glRotatef(120.0, 0.0, 0.0, 1.0);
	draw_blade();// draw the second blade

	glRotatef(120.0, 0.0, 0.0, 1.0);
	draw_blade();// draw the third blade

	glPopMatrix();                                                         //M5 pop

    // draw the helicopter's leg
	glPushMatrix();                                                        //M6 push
	glColor3f(0.06,0.06,0.06);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(-10.0, -4.0, -4.5);
	gluCylinder(cylind, 0.25, 0.25, // radius of top and bottom circle 
		9.0,              // height of the cylinder
		12,               // use 12-side polygon approximating circle
		3);               // divide it into 3 sections

	glTranslatef(0.0, 8.0, 0.0);
	gluCylinder(cylind, 0.25, 0.25,
		9.0,
		12,
		3);

	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0,2.0, 0.0);
	glPushMatrix();                                                         //M7 push
	gluCylinder(cylind, 0.25, 0.25,
		8.0,
		12,
		3);

	glTranslatef(0.0, 5.5, 0.0);
	gluCylinder(cylind, 0.25, 0.25,
		8.0,
		12,
		3);
	glRotatef(80.0, 0.0, 1.0, 0.0);
	gluCylinder(cylind, 0.25, 0.25,
		4.0,
		12,
		3);

	glTranslatef(0.0, -5.5, 0.0);
	gluCylinder(cylind, 0.25, 0.25,
		4.0,
		12,
		3);
	
	glTranslatef(-6.5, 0.0, 5.0);
	glRotatef(-160, 0.0, 1.0, 0.0);
	gluCylinder(cylind, 0.25, 0.25,
		4.0,
		12,
		3);
	glTranslatef(0.0, 5.5, 0.0);
	gluCylinder(cylind, 0.25, 0.25,
		4.0,
		12,
		3);
	glPopMatrix();                                                      //M7 pop


    // the trail of helicopter
	glPushMatrix();                                                     //M8 push
	glColor3f(0.08, 0.20, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(3.0, -5.0, -10.0);
	gluCylinder(cylind, 0.8, 0.8, // radius of top and bottom circle
		12.0,                     // height of the cylinder
		12,                       // use 12-side polygon approximating circle
		3);                       // Divide it into 3 sections
	glPopMatrix();                                                      //M8 pop

	glPushMatrix();                                                     //M9 push
	glColor3f(0.06,0.06,0.06);
    glTranslatef(4.0, -8.0, 5.0);
    glRotatef(90.0, 0.0, 0.0, 1.0);
	gluSphere(sphere, 0.8,   // radius
		12,                  // 12 slices
		12);                 // 12 stacks
	glRotatef(arm_ang, 1.0, 0.0, 0.0);

	// draw three blades
	glColor3f(1.0, 1.0, 1.0);
	glRotatef(bld_ang, 0.0, 0.0, 1.0);
	draw_blade();

	glRotatef(120.0, 0.0, 0.0, 1.0);
	draw_blade();

	glRotatef(120.0, 0.0, 0.0, 1.0);
	draw_blade();
	glPopMatrix();                                                      //M9 pop

    glPopMatrix();                                                      //M6 pop
    glPopMatrix();                                                      //MDE1 pop

    glPopMatrix(); // World                                             //M1 pop
}
void computeABaxes() {
    // b: y axis, c: w axis, a = b x c = [c0, 1.0, c2];
    b[0]= 0.0, b[1] = 1.0, b[2] = 0.0;
    c[0] = matrix[2], c[1] = matrix[6], c[2] = matrix[10];
    a[0] = c[2] / sqrt(c[0] * c[0] + c[2] * c[2]);
    a[1] = 0.0;
    a[2] = -1 * c[0] / sqrt(c[0] * c[0] + c[2] * c[2]);
}
void make_view(int mode) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  switch(mode){
  case 4:
    // perspective
    gluLookAt(eye[0], eye[1], eye[2], 
	      eye[0]-im[2][0], eye[1]-im[2][1], eye[2]-im[2][2],
	      im[1][0], im[1][1], im[1][2]);
    // store paramaters of model-view matrix to craete billboard
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    computeABaxes();
    break;

  case 1:       // x direction parallel projection
    gluLookAt(30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    computeABaxes();
    break;
  case 2:       // y direction parallel projection
    gluLookAt(0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    computeABaxes();
    break;
  case 3:       // z direction parallel projection
    gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    computeABaxes();
    break;
  }
}

void make_projection(int mode)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(mode == 4){
    gluPerspective(carmera_ang, (double) width/ (double) height, 1.5, 100.0);
  }else{
      if(width>height)
	glOrtho(-carmerax_ang, carmerax_ang, -carmerax_ang * (float)height/(float)width, 
		carmerax_ang * (float)height/(float)width, 
		-0.0, 100.0);
      else
	glOrtho(-carmerax_ang * (float)width/(float)height, 
		carmerax_ang * (float)width/(float)height, -carmerax_ang, carmerax_ang, 
		-0.0, 100.0);
  }
  glMatrixMode(GL_MODELVIEW);
  
}

// callback function of glutDisplayFunc
void display_func() {
    // clear previous frame and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // define directional light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, sun_diffuse);
    // glLightfv(GL_LIGHT1, GL_SPECULAR, spot_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, sun_direction);

    // define point light
    glLightfv(GL_LIGHT2, GL_DIFFUSE, point_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, point_specular);
    glLightfv(GL_LIGHT2, GL_POSITION, point_position);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, spot_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spot_specular);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);

    switch(mode) {
    case 0:
        make_view(4);
        make_projection(4);
        glViewport(width/2, 0, width/2, height/2);
        draw_helicopter();

        make_view(1);
        make_projection(1);
        glViewport(0, height/2, width/2, height/2);
        draw_helicopter();
        make_view(1);
        draw_view();

        make_view(2);
        make_projection(2);
        glViewport(width/2, height/2, width/2, height/2);
        draw_helicopter();
        make_view(2);
        draw_view();

        make_view(3);
        make_projection(3);
        glViewport(0, 0, width/2, height/2);
        draw_helicopter();
        make_view(3);
        draw_view();
        break;
    case 1:
        make_view(1);
        make_projection(1);
        glViewport(0, 0, width, height);
        draw_helicopter();
        make_view(1);
        draw_view();
        break;
    case 2:
        make_view(2);
        make_projection(2);
        glViewport(0, 0, width, height);
        draw_helicopter();
        make_view(2);
        draw_view();
        break;
    case 3:
        make_view(3);
        make_projection(3);
        glViewport(0, 0, width, height);
        draw_helicopter();
        make_view(3);
        draw_view();
        break;
    case 4:
        glViewport(0, 0, width, height);
        make_view(4);
        make_projection(4);
        draw_helicopter();
        break;
    }
    // swap the back buffer to the front
    glutSwapBuffers();
    return;
}
// callback function of glutIdleFunc
void idle_func()
{
    if (position[1] > 2.0)
    {
        bld_ang += 5.0;
        if (bld_ang > 360.0) {
            bld_ang -= 360.0;
            arm_ang += 3.0;
            if (arm_ang > 360.0) arm_ang -= 360.0;
        }
    }
    display_func();
}
// callback function of glutReshapeFunc
void reshape_func(int w, int h)
{
    width = w;
    height = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (double)w / (double)h, 1.5, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// call from main function
void keyboard_func(unsigned char key, int ix, int iy) {
    float x[3], y[3], z[3];

    if(key == 'q' || key == 'Q') exit(0);
    if(key == 'u') {
        arm_ang += 5.0;
        if(arm_ang > 360.0) arm_ang -= 360.0;
    }
    if(key == 'd'){
        bld_ang += 5.0;
        if(bld_ang > 360.0) bld_ang -= 360.0;
    }
    if(key == 'a') { // Counterclockwise rotate
        self_ang += 2.0;
        if(self_ang > 360.0) self_ang -= 360.0;
         printf("%f\n", -self_ang);
         printf("%f:%f:%f\n", spot_direction[0], spot_direction[1], spot_direction[2]);
    }
    if(key == 'e') { // clockwise rotate
        self_ang -= 2.0;
        if(self_ang < 0.0) self_ang += 360.0;
    }
    else if(key == 'x') { //繞x軸旋轉
        int temp = 0;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                position[i] += Rx[i][j] * position1[j][0];
            }
        }
        temp = -1 * position[1];
		position[1] = position[0];
		position[0] = temp;
    }
    else if (key == 'y'){//繞y軸旋轉
        int temp = 0;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				position[i] += Ry[i][j] * position1[j][0];
			}
		}
        temp = -1 * position[2];
		position[2] = position[0];
		position[0] = temp;
	}
    else if (key == 'z'){//繞z軸旋轉
        int temp = 0;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				position[i] += Rz[i][j] * position1[j][0];
			}
		}
        temp = -1 * position[0];
		position[0] = position[1];
		position[1] = temp;
	}
    else if(key == 'f') { // carmera 前進
        eyeDz += 0.5;
        for(int i = 0; i < 3; i++)
            eye[i] -= 0.5 * im[2][i];
    }
    else if(key == 'b') { // caemera 後退
        eyeDz -= 0.5;
        for(int i = 0; i < 3; i++)
            eye[i] += 0.5 * im[2][i];
    }
    else if(key == 'i') { // zoom in
        carmera_ang -= 0.2;
    }
    else if(key == 'o') { // zoom out
        carmera_ang += 0.2;
    }
    else if(key == 'p') { // pitching : according z axis of helicopter to rotate
        eyeAngx += 2.0;
        if(eyeAngx > 360.0) eyeAngx -= 360.0;
        for(int i = 0; i < 3; i++) {
            y[i] = im[1][i] * cos(5*PI/180.0) - im[2][i] * sin(5*PI/180.0);
            z[i] = im[2][i] * cos(5*PI/180.0) + im[1][i] * sin(5*PI/180.0);
        }
        for(int i = 0;i < 3; i++) {
            im[1][i] = y[i];
            im[2][i] = z[i];
        }
    }
    else if(key == 'P') {
        eyeAngx -= 2.0;
        if(eyeAngx < 0.0) eyeAngx += 360.0;
        for(int i = 0; i < 3; i++) {
            y[i] = im[1][i] * cos(5*PI/180.0) + im[2][i] * sin(5*PI/180.0);
            z[i] = im[2][i] * cos(5*PI/180.0) - im[1][i] * sin(5*PI/180.0);
        }
        for(int i = 0;i < 3; i++) {
            im[1][i] = y[i];
            im[2][i] = z[i];
        }
    }
    else if(key == 'h') { // yaw ( heading ) : according y axis of helicopter to rotate
        eyeAngy += 2.0;
        if(eyeAngy > 360.0) eyeAngy -= 360.0;
        for(int i = 0; i < 3; i++) {
            x[i] = im[0][i] * cos(5*PI/180.0) + im[2][i] * -sin(5*PI/180.0);
            z[i] = im[0][i] * sin(5*PI/180.0) + im[2][i] * cos(5*PI/180.0);
        }
        for(int i = 0;i < 3; i++) {
            im[0][i] = x[i];
            im[2][i] = z[i];
        }
    }
    else if(key == 'H') {
        eyeAngy -= 2.0;
        if(eyeAngy < 0.0) eyeAngy += 360.0;
        for(int i = 0; i < 3; i++) {
            x[i] = im[0][i] * cos(5*PI/180.0) + im[2][i] * sin(5*PI/180.0);
            z[i] = im[0][i] * -sin(5*PI/180.0) + im[2][i] * cos(5*PI/180.0);
        }
        for(int i = 0;i < 3; i++) {
            im[0][i] = x[i];
            im[2][i] = z[i];
        }
    }
    else if(key == 'r'){ // roll : according x axis of helicopter to rotate
        eyeAngz += 2.0;
        if(eyeAngz > 360.0) eyeAngz -= 360.0;
        for(int i = 0; i < 3; i++) {
            x[i] = im[0][i] * cos(5*PI/180.0) + im[1][i] * -sin(5*PI/180.0);
            y[i] = im[0][i] * sin(5*PI/180.0) + im[1][i] * cos(5*PI/180.0);
        }
        for(int i = 0;i < 3; i++) {
            im[0][i] = x[i];
            im[1][i] = y[i];
        }
    }
    else if(key == 'R') {
        eyeAngz -= 2.0;
        if(eyeAngz < 0.0) eyeAngz += 360.0;
        for(int i = 0; i < 3; i++) {
            x[i] = im[0][i] * cos(5*PI/180.0) + im[1][i] * sin(5*PI/180.0);
            y[i] = im[0][i] * -sin(5*PI/180.0) + im[1][i] * cos(5*PI/180.0);
        }
        for(int i = 0;i < 3; i++) {
            im[0][i] = x[i];
            im[1][i] = y[i];
        }
    }

    else if(key == '0') mode = 0;
    else if(key == '1') mode = 1;
    else if(key == '2') mode = 2;
    else if(key == '3') mode = 3;
    else if(key == '4') mode = 4;
    else if(key == '5') { // move left
        eyeDx -= 0.5;
        for(int i = 0; i < 3; i++)
            eye[i] += 0.5 * im[0][i]; 
    }
    else if(key == '6') { // move right
        eyeDx += 0.5;
        for(int i = 0; i < 3; i++)
            eye[i] -= 0.5 * im[0][i];
    }
    else if(key == '7') { // move up
        eyeDy += 0.5;
        for(int i = 0; i < 3; i++)
            eye[i] -= 0.5 * im[1][i];
    }
    else if(key == '8') { // move down
        eyeDy -= 0.5;
        for(int i = 0; i < 3; i++)
            eye[i] += 0.5 * im[1][i];
    }
    else if(key == '9') { // turn on/off sun light
        if(sun_switch)
            glDisable(GL_LIGHT1);
        else
            glEnable(GL_LIGHT1);
        sun_switch = !sun_switch;
    }
    else if(key == '/') { // turn on/off point light
        if(point_switch)
            glDisable(GL_LIGHT2);
        else
            glEnable(GL_LIGHT2);
        point_switch = !point_switch;
    }
    else if(key == '*') { // turn on/off spot light
        if(spot_switch)
            glDisable(GL_LIGHT0);
        else
            glEnable(GL_LIGHT0);
        spot_switch = !spot_switch;
    }
    else if(key == '-') { // move point light
        point_angle += 5.0;
    }
    else if(key == '[') { // switch directional light color
        // glEnable(GL_LIGHT1);
        if(sun_color == 0) { // red
            sun_diffuse[0] = 0.95,  sun_diffuse[1] = 0.05, sun_diffuse[2] = 0.05;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, sun_diffuse);
            sun_color = 1;
        }
        else if(sun_color == 1) { // green
            sun_diffuse[0] = 0.05,  sun_diffuse[1] = 0.95, sun_diffuse[2] = 0.05;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, sun_diffuse);
            sun_color = 2;
        }
        else if(sun_color == 2){ // blue
            sun_diffuse[0] = 0.05,  sun_diffuse[1] = 0.05, sun_diffuse[2] = 0.95;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, sun_diffuse);
            sun_color = 3;
        }
        else { // white
            sun_diffuse[0] = 0.95,  sun_diffuse[1] = 0.95, sun_diffuse[2] = 0.95;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, sun_diffuse);
            sun_color = 0;
        }
    }
    else if(key == '{') { // switch point light color
        // glEnable(GL_LIGHT2);
        if(point_color == 0) { // red
            point_diffuse[0] = 0.95,  point_diffuse[1] = 0.05, point_diffuse[2] = 0.05;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, point_diffuse);
            point_color = 1;
        }
        else if(point_color == 1) { // green
            point_diffuse[0] = 0.05,  point_diffuse[1] = 0.95, point_diffuse[2] = 0.05;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, point_diffuse);
            point_color = 2;
        }
        else if(point_color == 2){ // blue
            point_diffuse[0] = 0.05,  point_diffuse[1] = 0.05, point_diffuse[2] = 0.95;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, point_diffuse);
            point_color = 3;
        }
        else { // white
            point_diffuse[0] = 0.95,  point_diffuse[1] = 0.95, point_diffuse[2] = 0.95;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, point_diffuse);
            point_color = 0;
        }
    }
    else if(key == ']') {
        // glEnable(GL_LIGHT0);
        if(spot_color == 0) { // red
            spot_diffuse[0] = 0.95,  spot_diffuse[1] = 0.05, spot_diffuse[2] = 0.05;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, spot_diffuse);
            spot_color = 1;
        }
        else if(spot_color == 1) { // green
            spot_diffuse[0] = 0.05,  spot_diffuse[1] = 0.95, spot_diffuse[2] = 0.05;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, spot_diffuse);
            spot_color = 2;
        }
        else if(spot_color == 2){ // blue
            spot_diffuse[0] = 0.05,  spot_diffuse[1] = 0.05, spot_diffuse[2] = 0.95;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, spot_diffuse);
            spot_color = 3;
        }
        else { // white
            spot_diffuse[0] = 0.95,  spot_diffuse[1] = 0.95, spot_diffuse[2] = 0.95;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, spot_diffuse);
            spot_color = 0;
        }
    }
    else if(key == '}') {
        sun_intensity += 0.1f;
        sun_diffuse[0] *= sun_intensity, sun_diffuse[1] *= sun_intensity, sun_diffuse[2] *= sun_intensity;
        if(sun_intensity > 5.0)
            sun_intensity = 0.1f;
        glLightfv(GL_LIGHT1, GL_DIFFUSE, sun_diffuse);
    }
    else if(key == '?') {
        point_intensity += 0.1f;
        point_diffuse[0] *= point_intensity, point_diffuse[1] *= point_intensity, point_diffuse[2] *= point_intensity;
        if(point_intensity > 5.0)
            point_intensity = 0.1f;
        glLightfv(GL_LIGHT2, GL_DIFFUSE, point_diffuse);
    }
    else if(key == '.') {
        spot_intensity += 0.1f;
        spot_diffuse[0] *= spot_intensity, spot_diffuse[1] *= spot_intensity, spot_diffuse[2] *= spot_intensity;
        if(spot_intensity > 5.0)
            spot_intensity = 0.1f;
        glLightfv(GL_LIGHT0, GL_DIFFUSE, spot_diffuse);
    }
    else if(key == ':') {
        spot_cutoff += 0.5;
    }
    else if(key == ';') {
        sun_direction[0] += 0.5 *  cos(5 * PI / 180);
        sun_direction[2] -= 0.5 * sin(5 * PI / 180);
        glLightfv(GL_LIGHT1, GL_POSITION, sun_direction);
        // printf("%f:%f\n", sun_direction[0], sun_direction[1]);
    }
    else if(key == ',') {
        sun_direction[0] -= 0.5 *  cos(5 * PI / 180);
        sun_direction[2] += 0.5 * sin(5 * PI / 180);
        glLightfv(GL_LIGHT1, GL_POSITION, sun_direction);
        // printf("%f:%f\n", sun_direction[0], sun_direction[1]);
    }
    else if(key == '!') { // turn on/off fog
        if(fog_switch)
            glDisable(GL_FOG);
        else
            glEnable(GL_FOG);
        fog_switch = !fog_switch;
    }
    else if(key == '@') { // fog mode
        if(fog_mode == 0) {
            glFogi(GL_FOG_MODE, GL_LINEAR);
            fog_mode = 1;
        }
        else if(fog_mode == 1) {
            glFogi(GL_FOG_MODE, GL_EXP);
            fog_mode = 2;
        }
        else {
            glFogi(GL_FOG_MODE, GL_EXP2);
            fog_mode = 0;
        }
    }
    else if(key == '#') { // fog color
        fog_color[0] += 0.1, fog_color[1] += 0.1, fog_color[2] += 0.1;
        if (fog_color[0] >= 1.0)
            fog_color[0] = 0.15;
        if (fog_color[1] >= 1.0)
            fog_color[1] = 0.2;
        if (fog_color[2] >= 1.0)
            fog_color[2] = 0.2;
        glFogfv(GL_FOG_COLOR, fog_color);
    }
    else if(key == '$') {
        fog_density += 0.1;
        if (fog_density >= 1.0)
            fog_density = 0.1;
        glFogf(GL_FOG_DENSITY, fog_density);        
    }
    display_func();
}
// call from main()
void special_func(int key, int x, int y) {
    int flag = 0; // check helicopter fly or not
    if(key == GLUT_KEY_UP) { // fly
        if (position[1] <= 2.0 || (tilt_ang < 30 && up < 14)) { //起飛時要傾斜
            if (position[1] <= 2.0)
                up = 0;
            tilt_ang += 2.0;
            up++;
        }
        else {
            while (tilt_ang > 0)
                tilt_ang -= 2.0;
        }
        if (self_ang < 0)
        {
            position[1] -= 0.5 * sin(self_ang * PI / 180.0);
            position[1] += 0.5 * cos(self_ang * PI / 180.0);
        }
        else if (self_ang >= 0 && self_ang <= 90)
        {
            position[1] += 0.5 * sin(self_ang * PI / 180.0);
            position[1] += 0.5 * cos(self_ang * PI / 180.0);
        }
        else if (self_ang > 90 && self_ang <= 180)
        {
            position[1] += 0.5 * sin(self_ang * PI / 180.0);
            position[1] -= 0.5 * cos(self_ang * PI / 180.0);
        }
        else if (self_ang > 180 && self_ang <= 270)
        {
            position[1] -= 0.5 * sin(self_ang * PI / 180.0);
            position[1] -= 0.5 * cos(self_ang * PI / 180.0);
        }
        else if (self_ang > 270 && self_ang <= 360)
        {
            position[1] -= 0.5 * sin(self_ang * PI / 180.0);
            position[1] += 0.5 * cos(self_ang * PI / 180.0);
        }
        if (position[1] > 2.0)
            bld_ang += 0.5;
    }
    else if(key == GLUT_KEY_DOWN) { // landing
        if (tilt_ang > 0) {
            tilt_ang -= 2.0;
        }
        else if(tilt_ang <= 0)
            tilt_ang += 2.0;
        if (self_ang < 0)
        {
            position[1] += 0.5 * sin(self_ang * PI / 180.0);
            position[1] -= 0.5 * cos(self_ang * PI / 180.0);
        }
        else if (self_ang >= 0 && self_ang <= 90)
        {
            position[1] -= 0.5 * sin(self_ang * PI / 180.0);
            position[1] -= 0.5 * cos(self_ang * PI / 180.0);
        }
        else if (self_ang > 90 && self_ang <= 180)
        {
            position[1] -= 0.5 * sin(self_ang * PI / 180.0);
            position[1] += 0.5 * cos(self_ang * PI / 180.0);
        }
        else if (self_ang > 180 && self_ang <= 270)
        {
            position[1] += 0.5 * sin(self_ang * PI / 180.0);
            position[1] += 0.5 * cos(self_ang * PI / 180.0);
        }
        else if (self_ang > 270 && self_ang <= 360)
        {
            position[1] += 0.5 * sin(self_ang * PI / 180.0);
            position[1] -= 0.5 * cos(self_ang * PI / 180.0);
        }
    }
    else if(key == GLUT_KEY_LEFT) { // go ahead
        if(tilt_ang > -30)
            tilt_ang -= 2.0;
        position[0] += 0.5 * cos(self_ang * PI / 180.0);
        position[2] -= 0.5 * sin(self_ang * PI / 180.0);
    }
    else if(key == GLUT_KEY_RIGHT) { // back
        if(tilt_ang < 30)
            tilt_ang += 2.0;
        position[0] -= 0.5 * cos(self_ang * PI / 180.0);
        position[2] += 0.5 * sin(self_ang * PI / 180.0);
    }
    display_func();
}

int main(int argc, char** argv) {
    // initialize
    glutInit(&argc, argv);

    // depth mode
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // create window
    glutInitWindowSize(width, height);
    glutCreateWindow("Helicopter");
    
    // initialize environment
    myinit();

    // draw
    glutDisplayFunc(display_func);
    // add idle event
    glutIdleFunc(idle_func);
    // check window size change
    glutReshapeFunc(reshape_func);
    // check keyboard event
    glutKeyboardFunc(keyboard_func);
    // check special keyboard event
    glutSpecialFunc(special_func);
    // shader loop
    glutMainLoop();
}