#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "Class.h"
using namespace std;


Point startPoint = Point(-1,-1);  // Store the starting point for the rectangle
Point endPoint;    // Store the current endpoint as the mouse moves
Point previousEndPoint = Point(-1,-1);
int pointSize = 5;
int pointSizeEraser = 10;

int windowWidth = 1000;
int windowHeight = 600;
Slider redSlider,greenSlider,blueSlider;


vector<Button> buttons;
enum Mode{NONE,PENCIL,EXIT,LINE,SQUARE,OVAL,POINTSIZE,PLUS,MINUS,ERASER};
Mode mode = NONE;
Point initPoint;
bool isDrawing = false;
bool firstLine = false;
Line prevLine;



void drawEllipse(float x, float y, float xRadius, float yRadius, int segments) {
    glLineWidth(pointSize);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float xOffset = xRadius * std::cos(theta);
        float yOffset = yRadius * std::sin(theta);
        glVertex2f(x + xOffset, y + yOffset);
    }
    glEnd();
}
void drawButtons(){
   // Draw buttons as rectangles
    for (const Button& button : buttons) {
        glColor3f(0.8,0.8,0.8); // Button color
        // give opposite corners
        // ex: top-right and bottom-left..
        glRectd(button.x+button.width,button.y+button.height,button.x,button.y);

        // Draw button label
        glColor3f(0, 0, 0); // Text color
        if (mode == button.action){
            glColor3f(1,1,1);
        }
        if (button.label=="+" || button.label=="-"){
            glRasterPos2i(button.x+1 , (button.y+8 + (button.y + button.height)) / 2);
        }
        else{
            glRasterPos2i(button.x + 10, (button.y+8 + (button.y + button.height)) / 2);
        }
        for (char c : button.label) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }

    // Draw Sliders...
    redSlider.drawSlider();
    greenSlider.drawSlider();
    blueSlider.drawSlider();
    glFlush();
}
void setViewPort(){

    // Define the viewport and scissor to restrict drawing to the specified area
    //glViewport(100, 0, windowWidth, windowHeight);
    glScissor(200, 0, windowWidth, windowHeight);
    glEnable(GL_SCISSOR_TEST);
        glClearColor(0.8,0.8,0.8,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    glFlush();
}

void init(){
    glClearColor(0,0,0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    gluOrtho2D(0, windowWidth, windowHeight, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Initializing buttons..
    Button drawLineButton = {20, 50, 60, 20, "Pencil", 1};
    Button quitButton = {110, 50, 60, 20, "Quit", 2};
    Button lineButton = {20,80,60,20,"Line",3};
    Button squareButton = {110,80,60,20,"Square",4};
    Button ellipseButton = {20,110, 60, 20, "Oval", 5};
    Button pointSizeButton = {110,110, 40, 20, "Size", 6};
    Button plusButton = {151,110, 9, 20, "+", 7};
    Button minusButton = {161,110, 9, 20, "-", 8};
    Button eraserButton = {20,140, 60, 20, "Eraser", 9};
    redSlider = Slider(20,200,150,20,1);
    greenSlider = Slider(20,260,150,20,2);
    blueSlider = Slider(20,320,150,20,3);

    cout<<"1. x: "<<redSlider.sliderX<<endl;
    buttons.push_back(drawLineButton);
    buttons.push_back(quitButton);
    buttons.push_back(lineButton);
    buttons.push_back(squareButton);
    buttons.push_back(ellipseButton);
    buttons.push_back(pointSizeButton);
    buttons.push_back(plusButton);
    buttons.push_back(minusButton);
    buttons.push_back(eraserButton);

    //Setting view port..
    setViewPort();
    drawButtons();
}

bool inBoundary(int x,int y){
    return x>=200+2 && x<=windowWidth && y>=0 && y<=windowHeight;
}
void display() {
    glFlush();
}

void menu(int action){
    switch (action){
    case 1:
        //pencil
        mode = PENCIL;
        drawButtons();
        break;
    case 2:
        //exit
        mode = EXIT;
        drawButtons();
        exit(0);
    case 3:
        mode = LINE;
        drawButtons();
        break;
    case 4:
        mode = SQUARE;
        drawButtons();
        break;
    case 5:
        mode = OVAL;
        drawButtons();
        break;

    case 7:
        if (mode == ERASER){
            pointSizeEraser++;
        }
        else{
            pointSize++;
        }
        break;
    case 8:
        if (mode == ERASER){
            pointSizeEraser>1? pointSizeEraser-- : pointSizeEraser=1;
        }
        else{
            pointSize>1? pointSize-- : pointSize=1;
            cout<<"pointsize : "<<pointSize<<endl;
        }
        break;
    case 9:
        mode = ERASER;
        drawButtons();
        cout<<"eraser....";
        break;
    default:
        break;
    }

}

void drawLine(){
    isDrawing = false;
    firstLine = false;
}

void mouse(int button, int state, int x, int y) {
    //Check which button is pressed....
    if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON){
        cout<<"\nDown.."<<endl;

        //Draw line...
        if(mode==LINE && inBoundary(x,y)){
            initPoint = Point(x,y);
            isDrawing = true;
        }
        // Draw sliders....
        if(redSlider.isInsideSliderButton(x,y)){
            redSlider.drawing = true;
        }
        if(greenSlider.isInsideSliderButton(x,y)){
            greenSlider.drawing = true;
        }
        if(blueSlider.isInsideSliderButton(x,y)){
            blueSlider.drawing = true;
        }
        for (const Button& button: buttons){
            //top.. y<=button.y+height
            //bottom.. y>=button.y
            //left.. x>=button.x
            //right.. x<=button.x+button.width
            if (y<=button.y+button.height && y>=button.y && x>=button.x && x<=button.x+button.width){
                menu(button.action);
            }
        }
        if (mode==PLUS){
            pointSize++;
        }
        if (mode==MINUS){
            pointSize--;
        }
        if (mode==SQUARE && inBoundary(x,y)){
            startPoint = Point(x, y);  // Record the starting point
            isDrawing = true;
        }
        if (mode==OVAL && inBoundary(x,y)){
            startPoint = Point(x, y);  // Record the starting point
            isDrawing = true;
        }
        if (mode == ERASER && inBoundary(x,y)){
            glPointSize(pointSizeEraser);
            glColor3d(0.8,0.8,0.8);
            glBegin(GL_POINTS);
                glVertex2d(x,y);
            glEnd();
        }
    }
    else if (state== GLUT_UP){
        // Store the second point and draw the line..
        cout<<"\nDown.."<<endl;
        firstLine = false;
        isDrawing = false;
        redSlider.drawing = false;
        greenSlider.drawing = false;
        blueSlider.drawing = false;

        if (mode == SQUARE){
            glColor3f(redSlider.value/255.0f, greenSlider.value/255.0f, blueSlider.value/255.0f);
            if (startPoint.x != -1 && startPoint.y != -1 && inBoundary(x,y)){
                glRectd(startPoint.x, startPoint.y, x, y);
            }
            startPoint = Point(-1,-1);
            previousEndPoint = Point(-1,-1);
        }
        if (mode == OVAL && inBoundary(x,y)){
            if (startPoint.x != -1 && startPoint.y != -1 && previousEndPoint.x !=-1 && previousEndPoint.y!= -1){
                glColor4f(0.8,0.8,0.8,0.5);
                drawEllipse((previousEndPoint.x + startPoint.x)/2, (previousEndPoint.y + startPoint.y)/2, previousEndPoint.x-((previousEndPoint.x + startPoint.x)/2), previousEndPoint.y - ((previousEndPoint.y + startPoint.y)/2), 100);
            }
            glColor3f(redSlider.value/255.0f,greenSlider.value/255.0f, blueSlider.value/255.0f);
            if (startPoint.x != -1 && startPoint.y != -1){
                drawEllipse((x + startPoint.x)/2, (y + startPoint.y)/2, x-((x + startPoint.x)/2), y - ((y + startPoint.y)/2), 100);
            }
            startPoint = Point(-1,-1);
            previousEndPoint = Point(-1,-1);
        }
        if (mode == ERASER && inBoundary(x,y)){
            glPointSize(pointSizeEraser);
            glColor3d(0.8,0.8,0.8);
            glBegin(GL_POINTS);
                glVertex2d(x,y);
            glEnd();
        }
    }
}
void passiveMotion(int x, int y){
    if (inBoundary(x,y) && (mode==LINE || mode==SQUARE || mode==OVAL)){
        glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    }
    else{
        glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
    }
}
void liveUpdateSquare(){
    if (isDrawing) {
        if (startPoint.x != -1 && startPoint.y != -1 && previousEndPoint.x !=-1 && previousEndPoint.y!= -1){
            glColor4f(0.8,0.8,0.8,0.5);
            glRectd(startPoint.x, startPoint.y, previousEndPoint.x, previousEndPoint.y);
        }
        glColor3f(redSlider.value/255.0f,greenSlider.value/255.0f, blueSlider.value/255.0f);
        glRectd(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
    }

    glFlush();
}
void liveUpdateEllipse(){
    if (isDrawing) {
        if (startPoint.x != -1 && startPoint.y != -1 && previousEndPoint.x !=-1 && previousEndPoint.y!= -1){
            glColor4f(0.8,0.8,0.8,0.5);
            drawEllipse((previousEndPoint.x + startPoint.x)/2, (previousEndPoint.y + startPoint.y)/2, previousEndPoint.x-((previousEndPoint.x + startPoint.x)/2), previousEndPoint.y - ((previousEndPoint.y + startPoint.y)/2), 100);
        }
        glColor3f(redSlider.value/255.0f, greenSlider.value/255.0f, blueSlider.value/255.0f);
        drawEllipse((endPoint.x + startPoint.x)/2, (endPoint.y + startPoint.y)/2, endPoint.x-((endPoint.x + startPoint.x)/2), endPoint.y - ((endPoint.y + startPoint.y)/2), 100);
    }

    glFlush();
}
void motion(int x,int y){
    // Check if the point lies on the scissored portion...
    // Change to pencil mode..
    if(redSlider.drawing){
        cout<<"\nDrawing red slider.."<<endl;
        redSlider.computeSliderPosition(x);
        redSlider.drawSlider();
    }
    if(greenSlider.drawing){
        cout<<"\nDrawing green slider.."<<endl;
        greenSlider.computeSliderPosition(x);
        greenSlider.drawSlider();
    }
    if(blueSlider.drawing){
        cout<<"\n Drawing blue slider..."<<endl;
        blueSlider.computeSliderPosition(x);
        blueSlider.drawSlider();
    }

    if (mode==PENCIL && inBoundary(x,y)){
        glColor3d(redSlider.value/255.0f,greenSlider.value/255.0f, blueSlider.value/255.0f);
        glPointSize(pointSize);
        cout<<pointSize<<endl;
        glBegin(GL_POINTS);
            glVertex2d(x,y);
        glEnd();
    }
    //Draw the line interactively....
    else if (mode==LINE && inBoundary(x,y) && isDrawing){
        //Delete the previous line..
        glLineWidth(pointSize);
        cout<<pointSize<<endl;
        if(firstLine){
            glColor4f(0.8,0.8,0.8,0.5);
            glBegin(GL_LINES);
                glVertex2f(prevLine.p1.x,prevLine.p1.y);
                glVertex2f(prevLine.p2.x,prevLine.p2.y);
            glEnd();
        }
        cout<<"\nLine mode"<<endl;
        cout<<"\nDrawing line..."<<endl;
        glColor3f(redSlider.value/255.0f,greenSlider.value/255.0f, blueSlider.value/255.0f);
        glBegin(GL_LINES);
            glVertex2f(initPoint.x,initPoint.y);
            glVertex2f(x,y);
        glEnd();
        prevLine.p1.x = initPoint.x;
        prevLine.p1.y = initPoint.y;
        prevLine.p2.x = x;
        prevLine.p2.y = y;
        firstLine = true;
    }
    else if (mode==SQUARE && inBoundary(x,y)){
        if (isDrawing){
            endPoint = Point(x, y);  // Update the endpoint as the mouse moves
            liveUpdateSquare();  // Request a redraw to show the live rectangle
            previousEndPoint = Point(x, y);
        }
    }
    else if (mode==OVAL && inBoundary(x,y)){
        if (isDrawing) {
            endPoint = Point(x, y);  // Update the endpoint as the mouse moves
            liveUpdateEllipse();  // Request a redraw to show the live rectangle
            previousEndPoint = Point(x, y);
        }
    }
    else if (mode == ERASER && inBoundary(x,y)){
        glPointSize(pointSizeEraser);
        glColor3d(0.8,0.8,0.8);
        glBegin(GL_POINTS);
            glVertex2d(x,y);
        glEnd();
    }
    glFlush();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow("GLUT Drawing Application");
    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);
    glutMainLoop();
}
