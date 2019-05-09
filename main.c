#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

/* Setting up id's and inteval's for different timers. */

#define TIMER_ID_1 0
#define TIMER_INTERVAL_1 10.8

#define TIMER_ID_2 1
#define TIMER_INTERVAL_2 1

#define TIMER_ID_3 2
#define TIMER_INTERVAL_3 20

#define TIMER_ID_4 3
#define TIMER_INTERVAL_4 1

/* If you want to enable drawing of the coordinate system for orientational purposes,
 delete the comment characters around define HELP. */

/*#define HELP*/

/* Declarations of global variables. */

static int animation_ongoing, counter, game_over, game_lost;

static double left_rows_x_axis_movement, right_rows_x_axis_movement, 
            left_rows_z_axis_movement, right_rows_z_axis_movement, 
            ball_y_axis_movement, pi = 3.141592653589793, angle, k;

static GLfloat array_of_colors[100][4], ball_color[3], middle_row_colors[53][3];

static GLfloat red[3] = {0.929, 0.078, 0.047}, 
                green[3] = {0.196, 0.682, 0.160}, 
                blue[3] = {0.113, 0.070, 0.709}, 
                yellow[3] = {0.984,0.945,0.176};

/* Declarations of callback functions. */

static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_special_key(int key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer_1(int value);
static void on_timer_2(int value);
static void on_timer_3(int value);
static void on_timer_4(int value);

/* Declarations of auxiliary functions. */

static void pick_random_color(void);
static void draw_coordinate_system(void);
static void fill_array_of_colors(void);
static void draw_left_rows(void);
static void draw_right_rows(void);

int main(int argc, char **argv) {
    
    /* Lighting initialization. */
    
    GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1};
    GLfloat light_diffuse[] = {1, 1, 1, 1};
    GLfloat light_specular[] = {1, 1, 1, 1};
    GLfloat model_ambient[] = {0.1, 0.1, 0.1, 1};
    
    /* GLUT initialization. */
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    /* Creation of the window. */
    
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(200, 200);
    glutCreateWindow(argv[0]);
    
    /* Registrations of callback functions. */
    
    glutKeyboardFunc(on_keyboard);
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutSpecialFunc(on_special_key);
    
    /* OpenGL initialization. */
    
    glClearColor(0.752, 0.654, 0.925, 0);
    glEnable(GL_DEPTH_TEST);
    
    /* Enabling and setting up the lighting of the scene. */
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    
    /* At the begining of the program animation is turned off,
     game is not over, neither lost, counter is used for counting
     how many times the ball touches the platform and its initial
     value is zero, k is used for incrementing/decrementing y value
     of the ball, and angle is used for rotation around the platform
     once the game is lost. */
    
    animation_ongoing = 0;
    game_over = 0;
    game_lost = 0;
    counter = 0;
    k = 0.03;
    angle = 0;
    
    /* This function is used to pick 100 random colors and
     * remember them in a matrix, so that the cubes are 
     * colored the same way everytime display function is called. */ 
    
    fill_array_of_colors();
    
    /* Initial color of the ball is red. */
    
    ball_color[0] = red[0];
    ball_color[1] = red[1];
    ball_color[2] = red[2];
    
    /* Variables used to control z-axis and x-axis movement 
     * of the rows that construct the platform. */

    left_rows_x_axis_movement = 0;
    right_rows_x_axis_movement = 0;
    left_rows_z_axis_movement = 0;
    right_rows_z_axis_movement = 0;
    
    /* Variable used to control y-axis movement od the ball. */
    
    ball_y_axis_movement = 0;
   
    /* Program enters the main loop. */
    
    glutMainLoop();
    
    return 0;
}

static void on_reshape(int width, int height) {
    
    /* Adjusting the viewport. */
    
    glViewport(0, 0, width, height);
    
    /* Adjusting the projection. */
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 5, 30);
}

static void on_keyboard(unsigned char key, int x, int y) {
    
    switch(key) {
        case 27:
            /* Termination of the program. */
            exit(0);
            break;
        case 'm':
        case 'M':
            /* animation is set in motion. */
            if (!animation_ongoing && !game_over && !game_lost) {
                /* First, second and third timer are called. */
                glutTimerFunc(TIMER_INTERVAL_1, on_timer_1, TIMER_ID_1);
                glutTimerFunc(TIMER_INTERVAL_2, on_timer_2, TIMER_ID_2);
                glutTimerFunc(TIMER_INTERVAL_3, on_timer_3, TIMER_ID_3);
                animation_ongoing = 1;
            }
            break;
        case 's':
        case 'S':
            /* animation is suspended. */
            animation_ongoing = 0;
            break;
        case 'r':
        case 'R':
            /* the game is reset to its initial values. */
            k = 0.03;
            game_over = 0;
            game_lost = 0;
            counter = 0;
            angle = 0;
            left_rows_x_axis_movement = 0;
            right_rows_x_axis_movement = 0;
            left_rows_z_axis_movement = 0;
            right_rows_z_axis_movement = 0;
            ball_y_axis_movement = 0;
            ball_color[0] = red[0];
            ball_color[1] = red[1];
            ball_color[2] = red[2];
            fill_array_of_colors();
            animation_ongoing = 0;
            /* Display function is called. */
            glutPostRedisplay();
            break;
    }
}

static void on_special_key(int key, int x, int y) {
 
    switch(key) {
        case GLUT_KEY_LEFT:
            /* when the left arrow key is pressed ball changes color into red. */
            ball_color[0] = red[0];
            ball_color[1] = red[1];
            ball_color[2] = red[2];
            break;
        case GLUT_KEY_UP:
            /* when the up arrow key is pressed ball changes color into green. */
            ball_color[0] = green[0];
            ball_color[1] = green[1];
            ball_color[2] = green[2];
            break;
        case GLUT_KEY_RIGHT:
            /* when the right arrow key is pressed ball changes color into blue. */
            ball_color[0] = blue[0];
            ball_color[1] = blue[1];
            ball_color[2] = blue[2];
            break;
        case GLUT_KEY_DOWN:
            /* when the down arrow key is pressed ball changes color into yellow. */
            ball_color[0] = yellow[0];
            ball_color[1] = yellow[1];
            ball_color[2] = yellow[2];
            break;
    }
}

static void on_timer_1(int value) {
    
    /* if the value forwarded to timer is not corresponding,
     leave the timer.*/
    
    if (value != TIMER_ID_1)
        return;
    
    /* Controls the movement of the rows
     on the z-axis. */ 
    
    left_rows_z_axis_movement += 0.03778;
    
    right_rows_z_axis_movement += 0.03778;
    
    /* Display function is called. */
    
    glutPostRedisplay();
    
    /* Timer is called again. */
    
    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL_1, on_timer_1, TIMER_ID_1);
    }
}

static void on_timer_2(int value) {
    
    /* if the value forwarded to timer is not corresponding,
     leave the timer.*/
    
    if (value != TIMER_ID_2)
        return;
    
    /* Controls the movement of the rows on the x-axis
     * and stops the movemnet when it's in the middle of 
     the scene. */
    
    left_rows_x_axis_movement += 0.2;
    right_rows_x_axis_movement -= 0.2;
    
    if (right_rows_x_axis_movement <= -22)
        right_rows_x_axis_movement = -22;
    
    if (left_rows_x_axis_movement >= 22) 
        left_rows_x_axis_movement = 22;
  
    /* Display function is called. */
    
    glutPostRedisplay();
    
    /* Timer is called again. */
    
    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL_2, on_timer_2, TIMER_ID_2);
    }
}

static void on_timer_3(int value) {
    
    /* if the value forwarded to timer is not corresponding,
     leave the timer.*/
    
    if (value != TIMER_ID_3)
        return;
    
    /* Controls the movement of ball on y-axis. */
    
    ball_y_axis_movement = ball_y_axis_movement + k;
    
    /* If the ball reaches a certain point on y-axis 
     k changes the sign so that the movement of the ball
     can change the direction. This timer enables up/down
     movement of the ball. */
    
    if (ball_y_axis_movement >= 2)
        k = -0.05;
    
    if (ball_y_axis_movement <= 0.65) {
        k = 0.05;
        
        /* If the ball touches the platform counter is incremented. */
        counter += 1;
    }
    
    /* Timer is called again. */
    
    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL_3, on_timer_3, TIMER_ID_3);
    }
    
}

static void on_timer_4(int value) {
    
    /* if the value forwarded to timer is not corresponding,
     leave the timer.*/
    
    if (value != TIMER_ID_4)
        return;
   
    /* Controls the rotation around the platform
     and stops when a certain point is reached. */
    
    angle += pi/5;
    
    if (angle >= 150)
        angle = 150;
    
    /* Display function is called. */
    
    glutPostRedisplay();
    
    /* Timer is called again. */
    
    if (!animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL_4, on_timer_4, TIMER_ID_4);
    }
}

static void draw_coordinate_system(void) {
    
    /* Drawing the coordinate system.
     * This is an auxiliary function,
     * only made for orientational
     * purposes. */
    
    /* X-axis */
    
    glPushMatrix();
        glDisable(GL_LIGHTING);
        
        glColor3f(1,0,0);
        glBegin(GL_LINES);
            glVertex3f(-5,0,0);
            glVertex3f(5,0,0);
        glEnd();
        
        glEnable(GL_LIGHTING);
    glPopMatrix();
    
    /* Y-axis */
    
    glPushMatrix();
        glDisable(GL_LIGHTING);
        
        glColor3f(0,1,0);
        glBegin(GL_LINES);
            glVertex3f(0,-5,0);
            glVertex3f(0,5,0);
        glEnd();

        glEnable(GL_LIGHTING);
    glPopMatrix();
    
    /* Z-axis */
    
    glPushMatrix();
        glDisable(GL_LIGHTING);
        
        glColor3f(0,0,1);
        glBegin(GL_LINES);
            glVertex3f(0,0,-5);
            glVertex3f(0,0,5);
        glEnd();
        
        glEnable(GL_LIGHTING);
    glPopMatrix();
    
}

static void pick_random_color(void) {
    GLfloat x, y, z;
    
    /* Array of only four different colors is made. Red, green, blue and yellow, respectively. 
     Random color is to be picked only from that array. */ 

    static GLfloat array[4][3];
    
    array[0][0] = red[0];
    array[0][1] = red[1];
    array[0][2] = red[2];
    
    array[1][0] = green[0];
    array[1][1] = green[1];
    array[1][2] = green[2];
    
    array[2][0] = blue[0];
    array[2][1] = blue[1];
    array[2][2] = blue[2];
    
    array[3][0] = yellow[0];
    array[3][1] = yellow[1];
    array[3][2] = yellow[2];
                    
    /* We make sure that the index is in range [0,3]. */
                            
    int index = rand() % 4;
    
    x = array[index][0];
    y = array[index][1];
    z = array[index][2];

    glColor3f(x,y,z);
}

static void fill_array_of_colors(void) {
    
    /* This function fills the array(matrix) of colors by picking them randomly. 
     * This function is used so that each time the display 
     * function is called the colored cubes don't change color. */
    
    int i;
    for (i = 0; i < 100; i++) {
        pick_random_color();
        GLfloat color[4];
        glGetFloatv(GL_CURRENT_COLOR, color);
        array_of_colors[i][0] = color[0];
        array_of_colors[i][1] = color[1];
        array_of_colors[i][2] = color[2];
        array_of_colors[i][3] = color[3];
    }
    
}

static void draw_left_rows(void) {
    
    /* This function creates rows which are separated by one 
     * empty space. Each row consists of 7 cubes. Each one of the cubes
     * is translated so that they're on the left side and we cannot see them.
     * Lighting for each cube is set, and the cube is colored by one of the 
     * randomly picked colors. Afterwards, rows are set to move on the x-axis
     * and join with the right rows in the middle.
     */
    
    int i, j;
    for (j = 4; j <= 56; j += 2) {
        glPushMatrix();
            /* Moving rows on x-axis. */
            glTranslatef(left_rows_x_axis_movement,0,0);
            
            for (i = -25; i <= -19; i += 1) {
                glPushMatrix();
                    GLfloat x, y, z;
                    
                    /* We pick the color from the array of colors by 
                        accessing the index which depends on i and j, so
                        that it can be different for cubes with different
                        i and j values. */
                    
                    x = array_of_colors[(abs(i)*j)%100][0];
                    y = array_of_colors[(abs(i)*j)%100][1];
                    z = array_of_colors[(abs(i)*j)%100][2];
                    
                    /* if i = 22 it means we are currently making the middle row
                        for which we want to remember the colors, because the ball 
                        only touches the middle row.*/ 
                    
                    if (i == -22) {
                        middle_row_colors[j-4][0] = x;
                        middle_row_colors[j-4][1] = y;
                        middle_row_colors[j-4][2] = z;
                    }
                    
                    /* Material and shininess are set up for each cube. */
                    
                    GLfloat material_ambient[] = {1, 1, 1, 1};
                    GLfloat material_diffuse[] = {x, y, z, 1};
                    GLfloat material_specular[] = {0.7, 0.7, 0.7, 0.7};
                    GLfloat shininess[] = {30};
                    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
                    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
                    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
                    
                    /* The cube is translated so that it's on the left side 
                        and further from us. */
                    
                    glTranslatef(i,0,(-1)*j);
                    
                    /* The cube is made. */
                    
                    glutSolidCube(1);
                    
                glPopMatrix();
            }
            glPopMatrix();
        }
}

static void draw_right_rows(void) {
    
    /* This function creates rows which are separated by one 
     * empty space. Each row consists of 7 cubes. Each one of the cubes
     * is translated so that they're on the right side.
     * Lighting for each cube is set, and the cube is colored by one of the 
     * randomly picked colors. Afterwards, rows are set to move on the x-axis
     * and join with the left rows in the middle.
     */
    
    int j, i;
     for (j = 5; j <= 55; j += 2) {
        glPushMatrix();
            glTranslatef(right_rows_x_axis_movement,0,0);
            
            for (i = 19; i <= 25; i += 1) {
                glPushMatrix();
                    GLfloat x, y, z;
                    
                    /* We pick the color from the array of colors by 
                        accessing the index which depends on i and j, so
                        that it can be different for cubes with different
                        i and j values. */
                    
                    x = array_of_colors[(abs(i)*j)%100][0];
                    y = array_of_colors[(abs(i)*j)%100][1];
                    z = array_of_colors[(abs(i)*j)%100][2];
                    
                    /* if i = 22 it means we are currently making the middle row
                        for which we want to remember the colors, because the ball 
                        only touches the middle row.*/ 
                    
                    if (i == 22) {
                        middle_row_colors[j-4][0] = x;
                        middle_row_colors[j-4][1] = y;
                        middle_row_colors[j-4][2] = z;
                    }
                    
                    /* Material and shininess are set up for each cube. */
                    
                    GLfloat material_ambient[] = {1, 1, 1, 1};
                    GLfloat material_diffuse[] = {x, y, z, 1};
                    GLfloat no_material[] = {0.7, 0.7, 0.7, 0.7};
                    GLfloat shininess[] = {30};
                    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
                    glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
                    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
                    
                    /* The cube is translated so that it's on the right side
                        and further from us. */
                    
                    glTranslatef(i,0,(-1)*j);
                    
                    /* The cube is made. */
                    
                    glutSolidCube(1);
                    
                glPopMatrix();
            }
        glPopMatrix();
    }
}

static void on_display(void) {
    
    /* Setting up position of the light. */
    
    GLfloat light_position[] = {3, 5, 3, 0};
    
    /* Previous content of the window is deleted. */ 
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /* Adjusting the point of view. */
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1,7,8,0,0,0,0,1,0);
   
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
#ifdef HELP

    draw_coordinate_system();
   
#endif   
    
    if (game_lost) {
        /* If the game is lost we rotate around the platform until
         a certain point is reached. */
        glRotatef(angle, 0, 1, 0);  
        /* When that point is reached we write the sign "BETTER LUCK NEXT TIME :("*/
        if (angle == 150) {
            glPushMatrix();
                glDisable(GL_LIGHTING);
                glColor3f(0.7,0.0,0.0);
                char c[24] = {'B', 'E', 'T', 'T', 'E', 'R', ' ',
                                'L', 'U', 'C', 'K', ' ', 
                                'N', 'E', 'X', 'T', ' ',
                                'T', 'I', 'M', 'E', ' ', ':', '('};
                glRasterPos3f(3, 4, 0);
                int i;
                for (i = 0; i < 24; i++)
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c[i]);
                glEnable(GL_LIGHTING);
            glPopMatrix();
        }
    }
    
    int counter2;
    
    if (ball_y_axis_movement <= 0.65) {
        
        /* If the ball_y_axis_movement <= 0.65 it means that
         the ball has touched the platform, so we want to compare
         the color of the touched cube and the ball. But, before the 
         ball even reached the platform for the first time the counter
         value has already been 13, because the ball was originally positioned
         at zero on the y-axis and it incremented by 0.05 over time and 
         13*0.05 = 0.65. So we use an auxiliary counter (counter2).
         */
        
        counter2 = counter - 13;
        if (counter2 == 1) {
            /* If the ball has touched the first cube, compare the colors. */
            if (middle_row_colors[0][0] != ball_color[0] || 
                middle_row_colors[0][1] != ball_color[1] || 
                middle_row_colors[0][2] != ball_color[2]) {
                    
                    /* If the colors are not matching stop the animation,
                     call the timer for rotation around the platform
                     and set the game_lost variable to 1.*/
                    animation_ongoing = 0;
                    glutTimerFunc(TIMER_INTERVAL_4, on_timer_4, TIMER_ID_4);
                    game_lost = 1;
                }
        }
        else if (counter2 >= 2){
            /* If the ball has touched the cubes after the first one,
             compare the colors. Each time, the ball touches one cube, skips three
             and then touches the next one. So, (counter2-1)*4 is used to access
             the color of the corresponding cube. */
            if (middle_row_colors[(counter2-1)*4][0] != ball_color[0] ||
                middle_row_colors[(counter2-1)*4][1] != ball_color[1] || 
                middle_row_colors[(counter2-1)*4][2] != ball_color[2]) {
                    
                    /* If the colors are not matching stop the animation,
                     call the timer for rotation around the platform
                     and set the game_lost variable to 1.*/
                    animation_ongoing = 0;
                    glutTimerFunc(TIMER_INTERVAL_4, on_timer_4, TIMER_ID_4);
                    game_lost = 1;
                }
                if (counter2 == 14 && !game_lost) {
                    /* If we've come to this place it means that we've won.
                    We stop the animation and say that the game is over.*/
                    animation_ongoing = 0;
                    game_over = 1;
                }
        }
    }
    
    if (game_over) {
            /* If game_over = 1 it means we won, so we write the signe "YOU WON!!!"*/
            glPushMatrix();
                glDisable(GL_LIGHTING);
                glColor3f(0.0,0.7,0.0);
                char c[10] = {'Y', 'O', 'U', ' ', 'W', 'O', 'N', '!', '!', '!'};
                glRasterPos3f(0, 4, 3);
                int i;
                for (i = 0; i < 10; i++)
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c[i]);
                glEnable(GL_LIGHTING);
            glPopMatrix();
    }
    
    /* Drawing the ball. */
    
    glPushMatrix();
        /*Controling the movement of the ball.*/
        
        glTranslatef(0,ball_y_axis_movement,0);
        
        /* Initial position of the ball. */
        
        glTranslatef(0,0,1.6);
        
        /* Lighting and color of the ball. */
        
        if (!game_lost) {
            /* If the game is still on the color of the ball changes when certain keys are pressed. */
            
            GLfloat material_ambient[] = {1, 1, 1, 1};
            GLfloat material_diffuse[] = {ball_color[0], ball_color[1], ball_color[2], 1};
            GLfloat no_material[] = {1, 1, 1, 1};
            GLfloat shininess[] = {30};
            
            glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
        }
        else {
            
            /* If the game is lost the color of the ball changes to a dark gray. */
            
            GLfloat material_ambient[] = {1, 1, 1, 1};
            GLfloat material_diffuse[] = {0.3, 0.3, 0.3, 1};
            GLfloat no_material[] = {0, 0, 0, 1};
            GLfloat shininess[] = {0};
            
            glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
        }
        
        /* The ball is made. */
        
        glutSolidSphere(0.15,50,50);
        
    glPopMatrix();
    
    /* Drawing rows that come from the left side and moving them on z-axis. */
    
    glPushMatrix();
        glTranslatef(0,0,left_rows_z_axis_movement);
        draw_left_rows();
    glPopMatrix();
    
     /* Drawing rows that come from the right side and moving them on z-axis. */
    
    glPushMatrix();
        glTranslatef(0,0,right_rows_z_axis_movement);
        draw_right_rows();
    glPopMatrix();
    
    /* The new image is sent on screen. */
    
    glutSwapBuffers();
    
}