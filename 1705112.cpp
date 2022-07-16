#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

double camx,camy,camz, lookx,looky;
double cameraAnglex,cameraAngley;
int drawgrid;
int drawaxes;
double angle;
double side,radius;

struct point
{
	double x,y,z;
};

struct point cam,l,r,u;

double dot(struct point a,struct point b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

struct point cross(struct point a,struct point b){
    struct point r;
    r.x=a.y*b.z - a.z*b.y;
    r.y=a.z*b.x - a.x*b.z;
    r.z=a.x*b.y - a.y*b.x;
    return r;
}

struct point sum(struct point a,struct point b)
{
    struct point r;
    r.x=a.x + b.x;
    r.y=b.y + a.y;
    r.z=a.z + b.z;
    return r;
}

struct point sub(struct point a,struct point b)
{
    struct point r;
    r.x=a.x - b.x;
    r.y=a.y - b.y;
    r.z=a.z - b.z;
    return r;
}

struct point scale(struct point a,double s)
{
    a.x*=s;
    a.y*=s;
    a.z*=s;
    return a;
}

struct point rotate(struct point ax,struct point inp, double angle)
{
    return sum(scale(inp,cos(angle*pi/180)),scale(cross(ax,inp),sin(angle*pi/180)));
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

void draweighthSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*pi/2);
			points[i][j].y=r*sin(((double)j/(double)slices)*pi/2);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(1,0,0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
            //upper hemisphere
            glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
            glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
            glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
            glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
            }glEnd();
		}
	}
}

void drawcylinder(double radius, double height, int segments)
{
	struct point points[100][100];
	int i,j;
	double h;
	//generate points
	for(i=0;i<=25;i++)
	{
		h=height*sin(((double)i/(double)25)*(pi/2));
		for(j=0;j<=25;j++)
		{
			points[i][j].x=radius*cos(((double)j/(double)25)*pi/2);
			points[i][j].y=radius*sin(((double)j/(double)25)*pi/2);
			points[i][j].z=h/2;
		}
	}
	//draw quads using generated points
	for(i=0;i<25;i++)
	{
        glColor3f(0, 1, 0);
		for(j=0;j<25;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawsides()
{
    double t=side/2;
    double d=side/2-radius;
    struct point p[4]={{0, -1, 0},{0, 1, 0},{1, 0, 0},{-1, 0, 0}};

    glColor3f(1,1,1);
    glPushMatrix();
    glTranslatef(0,0,-t);
    drawSquare(d);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,t);
    drawSquare(d);
    glPopMatrix();

    for (int i = 0; i < 4; i++) {
		glPushMatrix();{
        glRotatef(90, p[i].x,p[i].y,p[i].z);
        glTranslatef(0, 0, t);
        drawSquare(d);
		}
		glPopMatrix();
	}
}
void drawcorners()
{
    double d=side/2-radius;

    for (int i = 0; i < 4; i++) {
		glPushMatrix();{
        glRotatef(90*i, 0,0,1);
        glTranslatef(d,d,d);
        draweighthSphere(radius,25,20);
		}
		glPopMatrix();
	}

    for (int i = 0; i < 4; i++) {
		glPushMatrix();{
        glRotatef(90*i, 0,0,1);
        glTranslatef(d,d,-d);
        glRotatef(180,1,1,0);
        draweighthSphere(radius,25,20);
        }
		glPopMatrix();
	}

}

void drawedge()
{
    double d=side/2-radius,c=side-2*radius;

    for (int i = 0; i < 4; i++) {
		glPushMatrix();{
        glRotatef(90*i, 0,0,1);
        glTranslatef(d,d,0);
        drawcylinder(radius,c,20);
        }
		glPopMatrix();
	}

    for (int i = 0; i < 4; i++) {
		glPushMatrix();{
        glRotatef(90*i, 0,0,1);
        glTranslatef(0,d,d);
        glRotatef(90,0,-1,0);
        drawcylinder(radius,c,20);
        }
		glPopMatrix();
	}

    for (int i = 0; i < 4; i++) {
		glPushMatrix();{
        glRotatef(90*i, 0,0,1);
        glTranslatef(0,d,-d);
        glRotatef(90,0,1,0);
        drawcylinder(radius,c,20);
        }
		glPopMatrix();
	}

}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}

void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks, int c)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        
        if(c==0)    glColor3f(1,1,0);
        else if(c==1) glColor3f(0,1,0);
        else if(c==2) glColor3f(1,0,0);
        else if(c==3) glColor3f(0,0,1);
        else if(c==4) glColor3f(1,0,1);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawSS()
{
    //glColor3f(1,0,0);
    drawSphere(15,24,20,0);
    glPushMatrix();
    {
    glColor3f(1,1,0);
    //glRotatef(angle,0,0,1); 
    glTranslatef(100*cos(angle),60*sin(angle),0);
    //glRotatef(2*angle,0,0,0);
    drawSphere(12,24,20,1);
    }
    glPopMatrix();
    //glColor3f(0,1,1);
    glPushMatrix();
    {
    glRotatef(-angle,0,0,1);
    glTranslatef(150*cos(angle),80*sin(angle),0);
    //glRotatef(2*angle,0,0,1);
    drawSphere(10,24,20,2);
     }
    glPopMatrix();
    
    glPushMatrix();
    {
    glRotatef(angle,0,0,1);
    glTranslatef(180*cos(angle),100*sin(angle),0);
    //glRotatef(2*angle,0,0,1);
    drawSphere(8,24,20,3);

    glRotatef(angle,0,0,1);
    glTranslatef(15,0,0);
    //glRotatef(4*angle,0,0,1);
    drawSphere(5,24,20,4); 
     }
    glPopMatrix();
    //glColor3f(0,0,1);   
    //glColor3f(1,1,1);
    
}



void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			l=rotate(u,l,2.0);
            r=cross(l,u);
			break;
		case '2':
			l=rotate(u,l,-2.0);
            r=cross(l,u);
			break;
		case '3':
            l=rotate(r,l,2.0);
            u=cross(r,l);
            break;
		case '4':
			l=rotate(r,l,-2.0);
            u=cross(r,l);
			break;
		case '5':
			r=rotate(l,r,-2.0);
            u=cross(r,l);
			break;
		case '6':
			r=rotate(l,r,2.0);
            u=cross(r,l);
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key // move back
			cam=sub(cam,l);
			break;
		case GLUT_KEY_UP:		// up arrow key // move forward
			cam=sum(cam,l);
			break;

		case GLUT_KEY_RIGHT: // move r8
			cam=sum(cam,r);
			break;
		case GLUT_KEY_LEFT: //move left
			cam=sub(cam,r);
			break;

		case GLUT_KEY_PAGE_UP: //move up
			cam=sum(cam,u);
			break;
		case GLUT_KEY_PAGE_DOWN: //move down
			cam=sub(cam,u);
			break;
		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
            if(radius<side/2) radius++;
			break;
		case GLUT_KEY_END:
            if(radius>0) radius--;
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), camz,		0,0,0,		0,0,1);
	gluLookAt(cam.x,cam.y,cam.z,	cam.x+l.x,cam.y+l.y,cam.z+l.z,	u.x,u.y,u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects
	drawAxes();
	//drawGrid();
	
    glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
    cam={200,200,0};
    u={0,0,1};
    r={-1/sqrt(2),1/sqrt(2),0};
    l={-1/sqrt(2),-1/sqrt(2),0};
    side=50;
    radius=7;


	/* Use depth buffering for hidden surface elimination. */
	//glEnable(GL_DEPTH_TEST);

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
