#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <GL/glut.h>
#include <time.h>
#include <unistd.h>
#define NUM_SPHERES 10

int first,bfs_window_value;
int no_of_nodes,backspace,count,display_matrix,change_previous_value,input_value=-1;
int node_matrix[10][10],x[10],y[10];
int highlight_x0,highlight_x1,highlight_y0,highlight_y1;
int highlight_i,highlight_j;
int x_point,y_point,old_x_point,old_y_point;
int old_x, old_y;
float spin_x = 0;
float spin_y = 0;
int visited_bfs[20],visited_dfs[20];
int visit[10];
int bfs_algorithm;

typedef struct {
    float x, y, z;
    float radius;
} PSsphere;
PSsphere spheres[NUM_SPHERES];

float colors[][3] = {
    {1,1,1},{0,0,1},{0,0.55,0.75},{0,1,1},{0.5,0,0.5},{1,0,1},{1,0.5,0},{1,1,1},{0.5,1,0}
};
struct points{
    float x;
    float y;
    float z;
};
struct direction_ratio{
    float x;
    float y;
    float z;
};
// GLUT_BITMAP_8_BY_13
// GLUT_BITMAP_9_BY_15
// GLUT_BITMAP_TIMES_ROMAN_10
// GLUT_BITMAP_TIMES_ROMAN_24
// GLUT_BITMAP_HELVETICA_10
// GLUT_BITMAP_HELVETICA_12
// GLUT_BITMAP_HELVETICA_18
void drawTextLarge( float x, float y, int r, int g, int b,const char *string ){
	int j = strlen( string );
	glColor3f( r, g, b );
	glRasterPos2f( x, y );
	for( int i = 0; i < j; i++ ) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i] );
	}
}

void drawTextSmall( float x, float y, int r, int g, int b,const char *string ){
	int j = strlen( string );
	glColor3f( r, g, b );
	glRasterPos2f( x, y );
	for( int i = 0; i < j; i++ ) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, string[i] );
	}
}

void drawLines(int u,int v){
    glBegin(GL_LINES);
    glColor3f(1,0,1);
    glPointSize(20);
    glVertex3f(spheres[u].x,spheres[u].y,spheres[u].z);
    glVertex3f(spheres[v].x,spheres[v].y,spheres[v].z);
    glEnd();
    glFlush();
}

void dfs(int a[10][10],int n,int source){
	int i,j,index=0;
	visit[source] = 1;
	visited_dfs[index++]=source;
	for(j=1;j<=n;j++)
	{
		if(visit[j]==0 && a[source][j]==1){
			visited_dfs[index++] = j;
			dfs(a,n,j);
		}
	}
	// for(j=1;j<=n;j++)
	// 	if(visited_dfs[j]==0)
	// 		return 0;
	// return 1;
}

void bfs(int a[10][10],int n,int source){
	int k,f,r,u,v,i;
	int q[50];
	for(i=1;i<=n;i++)
		visited_bfs[i]=0;
	f=r=0;
	visited_bfs[source]=1;
	q[r]=source;
	while(f<=r)
	{
		u=q[f];
		f++;
		for(v=1;v<=n;v++)
		{
			if((a[u][v]==1) && (visited_bfs[v]==0))
			{
				visited_bfs[v]=1;
				r++;
				q[r]=v;
			}
		}
	}
}

void generate_spere(){
    int i,j;
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(spin_y, 1, 0, 0);
    glRotatef(spin_x, 0, 1, 0);
    glEnable(GL_LIGHTING);
	for (i = 0; i < no_of_nodes; i++) {
	    glPushMatrix();
	    glTranslatef(spheres[i].x, spheres[i].y, spheres[i].z);
	    glColor3fv(colors[i]);
	    glutSolidSphere(spheres[i].radius, 20,20);
	    glPopMatrix();
	}
    glDisable(GL_LIGHTING);
	if(bfs_algorithm == 1)
    {
		for(i=0;i<no_of_nodes;i++)
			for(j=0;j<no_of_nodes;j++)
				if(node_matrix[i][j] == 1)
                {
					drawLines(i,j);

                }
	}
	else{
		for(i=0;i<no_of_nodes;i++)
			if(visited_dfs[i]<no_of_nodes && visited_dfs[i+1]<no_of_nodes)
				drawLines(visited_dfs[i],visited_dfs[i+1]);
	}
    glColor4f(0, 0.3, 0.5, 0.3);
    glBegin(GL_QUADS);
    glVertex3f(4, -0.2, 4);
    glVertex3f(-4, -0.2, 4);
    glVertex3f(-4, -0.2, -4);
    glVertex3f(4, -0.2, -4);
    glEnd();
    glPopMatrix();
    glutSwapBuffers();
}

void reshape_sphere_window(int width, int height){
    float black[] = { 0, 0, 0, 0 };
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(120, -1, 2, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 1, 3, 0, 1, 0, 0, 1, 0);
    // glFogfv(GL_FOG_COLOR, black);
    // glFogf(GL_FOG_START, 2.5);
    // glFogf(GL_FOG_END, 4);
    // glEnable(GL_FOG);
    // glFogi(GL_FOG_MODE, GL_LINEAR);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
}

void motion_sphere_window(int x, int y){
    spin_x = x - old_x;
    spin_y = y - old_y;
    glutPostRedisplay();
}

void intro_display();
int index_value;
void keyboard_sphere_window(unsigned char key, int x, int y){
    switch (key)
    {
    	case 'q':
    	case 'Q':
            exit(0);
		case 'p':
		case 'P':
			if(bfs_algorithm == 1){
				if(visited_bfs[index_value] == 1){
					colors[index_value][0] = 0;
					colors[index_value][1] = 1;
					colors[index_value][2] = 0;
				}
				else{
					colors[index_value][0] = 1;
					colors[index_value][1] = 0;
					colors[index_value][2] = 0;
				}
				index_value++;
				glutPostRedisplay();
			}
			else{
				if(visit[index_value] == 1){
					colors[index_value][0] = 0;
					colors[index_value][1] = 1;
					colors[index_value][2] = 0;
				}
				else{
					colors[index_value][0] = 1;
					colors[index_value][1] = 0;
					colors[index_value][2] = 0;
				}
				index_value++;
				glutPostRedisplay();
			}
			break;
    }
}

void generate_points(int n){
    int x,y,z,i,j,k,count;
    float x_dr,y_dr,z_dr;
    float ratio_x,ratio_y,ratio_z;

    struct points p[n];
    struct direction_ratio dr[n][n];
    srand(time(NULL));

	start:
    for(i=0;i<n;i++)
    {
        //Generate Random Points
        p[i].x = rand() % n;
        p[i].y = rand() % n;
        p[i].z = rand() % n;

        //Assign root Node
        if(i == 0){
            p[i].x = 0;
            p[i].y = 0;
            p[i].z = n;
        }

        //Do not generate same point
        count = 0;
        for(j=0;j<i;j++){
            if(p[i].x == p[j].x)
                count++;
            if(p[i].y == p[j].y)
                count++;
            if(p[i].z == p[j].z)
                count++;
            if(count == 3)
                goto start;
        }

        //Generate Direction Ratios
        for(j=0;j<n;j++)
        {
            if(i==j)
                dr[i][j].x = dr[i][j].y = dr[i][j].z = 0;
            else
            {
                dr[j][i].x = dr[i][j].x = p[j].x - p[i].x;
                dr[j][i].y = dr[i][j].y = p[j].y - p[i].y;
                dr[j][i].z = dr[i][j].z = p[j].z - p[i].z;
            }
        }
    }

    //Check ratios
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            for(k=0;k<n;k++)
            {
                if(i!=j && j!=k && k!=i){
                    ratio_x = (float) dr[i][j].x / dr[j][k].x;
                    ratio_y = (float) dr[i][j].y / dr[j][k].y;
                    ratio_z = (float) dr[i][j].z / dr[j][k].z;
                    if(ratio_x == ratio_y && ratio_y == ratio_z)
                        goto start;
                }
            }

    for(j=0;j<n;j++)
    {   spheres[j].x = p[j].x/2;
        spheres[j].y = p[j].y/2;
        spheres[j].
z = p[j].z/2;
        spheres[j].radius = 0.1;
    }
}

void menu(int i){
	if(i==1)
		exit(0);
}

void display_algorithm(){
	int i,j;
	glutInitWindowPosition(0, 0);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("BFS/DFS");
	generate_points(no_of_nodes);
    glutDisplayFunc(generate_spere);
	if(bfs_algorithm == 1)
		bfs(node_matrix,no_of_nodes,0);
	if(bfs_algorithm == 0)
		dfs(node_matrix,no_of_nodes,0);
    glutReshapeFunc(reshape_sphere_window);
    glutMotionFunc(motion_sphere_window);
    glutKeyboardFunc(keyboard_sphere_window);
	glutCreateMenu(menu);
	glColor3f(1,1,1);
	glutAddMenuEntry("Quit",1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glColor3f(0,0,0);
	glutPostRedisplay();
	glFlush();
}

void rotate_sphere_window(int button, int state, int x, int y){
    old_x = x;
    old_y = y;
	if(button == GLUT_RIGHT_BUTTON)
		bfs_algorithm = 1;
	if(button == GLUT_LEFT_BUTTON)
		bfs_algorithm = 0;
	display_algorithm();
}

void populate_matrix(){
	if(change_previous_value == 1){
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		glVertex2f(x[old_x_point]+1,y[old_y_point]+1);
		glVertex2f(x[old_x_point+1]-1,y[old_y_point]+1);
		glVertex2f(x[old_x_point+1]-1,y[old_y_point+1]-1);
		glVertex2f(x[old_x_point]+1,y[old_y_point+1]-1);
		glEnd();
	}
	if(x_point <= no_of_nodes && y_point >= 0){
		glColor3f(1,1,0);
		glBegin(GL_POLYGON);
		glVertex2f(x[x_point]+1,y[y_point]+1);
		glVertex2f(x[x_point+1]-1,y[y_point]+1);
		glVertex2f(x[x_point+1]-1,y[y_point+1]-1);
		glVertex2f(x[x_point]+1,y[y_point+1]-1);
		glEnd();
		glFlush();
	}
}

void generate_matrix(){
	int x0 = (1000 - (no_of_nodes*50)) /2 ,y0 = 400, dx = 50,dy = 50;
	int i,j,temp_x0,temp_y0;
	char str[10]="";
	for(i=0;i<=no_of_nodes;i++)
		x[i]=x0+i*dx;
	for(j=0;j<=no_of_nodes;j++)
		y[j]=y0+j*dy;
	for(i=0;i<no_of_nodes;i++)
	{
		str[0] = (char) (i+1+48);
		drawTextSmall(x0-15, y0+(no_of_nodes-i)*dy-25, 0, 0, 0, str);
		drawTextSmall(x0+25+i*dx, y0+no_of_nodes*dy+15, 0, 0, 0, str);
	}
	glColor3f(0, 0, 1);
	for(i=0;i<no_of_nodes;i++)
	{
		temp_x0=x0+i*dx;
		for(j=0;j<no_of_nodes;j++)
		{
			temp_y0=y0+(no_of_nodes-1-j)*dy;
			if(node_matrix[j][i] != -1){
				str[0] = (char) (node_matrix[j][i] + 48);
				drawTextLarge(temp_x0+13,temp_y0+13,0,0,0,str);
			}
			glColor3f(0, 0, 1);
			glBegin(GL_LINE_LOOP);
			glVertex2f(x[i],y[j]);
			glVertex2f(x[i],y[j+1]);
			glVertex2f(x[i+1],y[j+1]);
			glVertex2f(x[i+1],y[j]);
			glEnd();
			glFlush();
		}
	}
	drawTextLarge(440,y[no_of_nodes]+50,0,0,0,"Enter Values");
	populate_matrix();
}

void keyboard_instructions(unsigned char key, int x, int y){
    switch (key)
    {
    	case 'q':
    	case 'Q':
            exit(0);
		case 'n':
		case 'N':
			intro_display();
			glutPostRedisplay();
			break;
    }
}

void instructions(){
		glClearColor(1,1,1,1);
	    glClear(GL_COLOR_BUFFER_BIT);
		drawTextLarge(300,650,0,0,0,"Right click for BFS");
		drawTextLarge(300,600,0,0,0,"Left click for DFS");

		drawTextLarge(500,300,0,0,1,"Instructions for use:");
		drawTextLarge(500,270,0,0,1,"-Press P to identify the next visited node");
		drawTextLarge(500,240,0,0,1,"-All nodes reachable from root node turn green");
		drawTextLarge(500,210,0,0,1,"-All nodes not reachable from root node turn red");
		drawTextLarge(500,180,0,0,1,"-Press N to re enter values");
		drawTextLarge(500,150,0,0,1,"-Press Q to quit");
	    glutMouseFunc(rotate_sphere_window);
		glFlush();
}

void intro_display();
void intro_keyboard(unsigned char,int,int);
void input_matrix_keyboard(unsigned char key,int x,int y){
	switch(key)
	{
		case 'n':
		case 'N':
			glutKeyboardFunc(intro_keyboard);
			intro_display();
			break;
		case 'x':
		case 'X':
			if(highlight_i > no_of_nodes*no_of_nodes)
				highlight_i = no_of_nodes*no_of_nodes;
			if(highlight_i > 0){
				old_x_point = highlight_i % no_of_nodes;
				old_y_point = highlight_j - (highlight_i / no_of_nodes);
				highlight_i--;
				x_point = highlight_i % no_of_nodes;
				y_point = highlight_j - (highlight_i / no_of_nodes);
				node_matrix[no_of_nodes-old_y_point-1][old_x_point] = -1;
				change_previous_value = 1;
				generate_matrix();
			}
			break;
		case '0':
		case '1':
			if(highlight_i <= no_of_nodes*no_of_nodes)
			{
				input_value = (int)key - 48;
				old_x_point = highlight_i % no_of_nodes;
				old_y_point = highlight_j - (highlight_i / no_of_nodes);
				highlight_i++;
				x_point = highlight_i % no_of_nodes;
				y_point = highlight_j - (highlight_i / no_of_nodes);
				node_matrix[no_of_nodes-old_y_point-1][old_x_point] = input_value;
				change_previous_value = 0;
				generate_matrix();
			}
			break;
		case 'q':
		case 'Q':
			exit(0);
		case 'c':
		case 'C':
			int count_nodes = 0;
			for(int i=0;i<no_of_nodes;i++)
				for(int j=0;j<no_of_nodes;j++)
					if(node_matrix[i][j]!=-1)
						count_nodes++;
			if(count_nodes == no_of_nodes*no_of_nodes)
				instructions();
	}
}

void input_matrix_display(){
	glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
	drawTextLarge(500,250,1,0,0,"Press X to change previous value");
	drawTextLarge(500,200,1,0,0,"Press N to change number of nodes");
	drawTextLarge(500,150,1,0,0,"Press C to continue after entering all values");
	drawTextLarge(500,100,1,0,0,"Press Q to quit");
	glutKeyboardFunc(input_matrix_keyboard);
	highlight_i = 0;
	highlight_j = no_of_nodes - 1;
	x_point = highlight_i % no_of_nodes;
	y_point = highlight_j - (highlight_i / no_of_nodes);
	generate_matrix();
	glFlush();
}

void intro_keyboard(unsigned char key,int x,int y){
    backspace = 0;
    switch (key) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if(count < 1){
                no_of_nodes = (int)key - 48 ;
            }
            count++;
            glutPostRedisplay();
            break;
		case 'x':
		case 'X':
	        backspace = 1;
	        no_of_nodes = count = 0;
	        glutPostRedisplay();
			break;
		case 'c':
		case 'C':
			if(no_of_nodes > 0)
				input_matrix_display();
			break;
		case 'q':
		case 'Q':
			exit(0);
    }
}

void myinit(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,1000,0,1000);
}

void intro_display(){
	for(int i=0;i<10;i++)
		for(int j=0;j<10;j++)
			node_matrix[i][j]=-1;
	glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    char value_str[10];
    value_str[0] = (char)(no_of_nodes + 48);
    drawTextLarge(300,800,0,0,0,"Graphical DFS & BFS Algorithm");
    drawTextLarge(700,950,0,0,0,"Mohein : 1BY15CS053");
    drawTextLarge(700,900,0,0,0,"Tanvi\t  : 1BY15CS092");
    drawTextLarge(200,500,0,0,0,"Enter number of nodes : ");
    if(backspace == 0 && no_of_nodes > 0){
        drawTextLarge(450,500,0,0,0,value_str);
        if(count > 1)
            drawTextLarge(300,400,0,0,0,"Only single digit nodes are allowed");
    }
    else
        drawTextLarge(450,500,0,0,0,"");
	drawTextLarge(500,225,1,0,0,"Press X for backspace");
	drawTextLarge(500,175,1,0,0,"Press C to continue after entering all values");
	drawTextLarge(500,125,1,0,0,"Press Q to quit");
	glFlush();
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_SINGLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1000, 1000);
    first = glutCreateWindow("Graphical Representation of BFS & DFS Algorthm");
    glutDisplayFunc(intro_display);
    glutKeyboardFunc(intro_keyboard);
    myinit();
    glutMainLoop();
    return 0;
}

