#include<iostream>
using namespace std;

class Button {
public:
    int x, y, width, height;
    string label;
    int action;
};

class Point{
public:
    int x,y;
    Point(){
    }
    Point(int x,int y){
        this->x = x;
        this->y = y;
    }
};

class Line{
public:
    Point p1,p2;
    Line(){

    }
    Line(Point p1,Point p2){
        this->p1 = p1;
        this->p2 = p2;
    }
};

class Slider{
public:
    int sliderX,sliderY,sliderWidth,sliderHeight,sliderButtonX,sliderPosition;
    int color;
    float r,g,b;
    bool drawing = false;
    float value;
    Slider(){
        value = 0;
    }
    Slider(int sliderX, int sliderY, int sliderWidth, int sliderHeight,int color){
        this->sliderX = sliderX;
        this->sliderY = sliderY;
        this->sliderWidth = sliderWidth;
        this->sliderHeight = sliderHeight;
        this->color = color;
        r = 0.0;g = 0.0;b = 0.0;
        setColor();
    }

    void setColor(){
        if (color==1){
            r = 1.0;
        }
        else if (color==2){
            g = 1.0;
        }
        else
            b = 1.0;
    }
    bool isInsideSliderButton(int x,int y){
        sliderButtonX =  sliderX + static_cast<int>((static_cast<float>(value) / 255.0f) * sliderWidth);
        return x >= sliderButtonX - 5 && x <= sliderButtonX + 5 && y >= sliderY - 5 && y <= sliderY + sliderHeight + 5;
    }
    void drawSlider(){
        glColor3f(0.6, 0.6, 0.6);  // Slider track color
        glRecti(sliderX, sliderY, sliderX + sliderWidth, sliderY + sliderHeight);

        // Calculate the position of the slider button based on the red value
        sliderButtonX = sliderX + static_cast<int>((static_cast<float>(value) / 255.0f) * sliderWidth);

        glColor3f(r, g, b);  // Slider button color (red)
        glRecti(sliderButtonX - 5, sliderY , sliderButtonX + 5, sliderY + sliderHeight );
        glFlush();
    }

    void computeSliderPosition(int x){
        sliderPosition = x - sliderX;
        if(sliderPosition<=0){
            value = 0;
        }
        else if (sliderPosition>=sliderWidth){
            value = 255;
        }
        else{
            value = static_cast<int>((static_cast<float>(sliderPosition) / sliderWidth * 255));
        }
    }
};
