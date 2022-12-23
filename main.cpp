#include "stdio.h"
#include "iostream"
#include "time.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SFML/Graphics.hpp>

int reader(const char *file, char *arr);
int writer(const char *file);
bool buttonPressed(sf::Vector2i p, sf::RectangleShape button);
sf::RenderWindow window(sf::VideoMode(100, 450), "aLi");
int flag = 1;
typedef struct value
{
    char charge[2];
    int noob : 1;
    char brightness[2];
    int noob2 : 1;
    char volume[2];
} value;

value v;

typedef struct button
{
    sf::RectangleShape button;
    float sizex;
    float sizey;
    float posx;
    float posy;
    sf::Color color;
} button;

void setButton(sf::RectangleShape button, float sizex, float sizey, float posx, float posy, sf::Color color)
{
    button.setSize(sf::Vector2f(sizex, sizey));
    button.setPosition(sf::Vector2f(posx, posy));
    button.setFillColor(color);
}
bool buttonPressed(sf::Vector2i p, sf::RectangleShape button)
{
    sf::Vector2f size = button.getSize();
    sf::Vector2f pp;
    pp = button.getPosition();
    float limitx = size.x + pp.x;
    float limity = size.y + pp.y;
    int returnValue = 0;
    if ((p.x < limitx && p.x > pp.x) && (p.y < limity && p.y > pp.y))
    {
        returnValue = 1;
    }
    return returnValue;
}
int reader(const char *file, char *arr)
{
    FILE *f;
    char ch;
    int i = 0;

    f = fopen(file, "r");

    if (NULL == f)
    {
        printf("File didnt load\n");
    }

    while (i <= 1)
    {
        ch = fgetc(f);
        arr[i] = ch;

        if (i == 1)
        {
            arr[2] = '%';
        }
        i++;
    }

    fclose(f);
    return 0;
}

int writer(const char *file, int value)
{
    FILE *f;
    int n;
    char ch;

    f = fopen(file, "w");
    if (NULL == f)
    {
        printf("File didnt load\n");
        return 1;
    }
    fprintf(f, "%d\n", value);
    fclose(f);
    return 0;
}

void brightness(const char *file,
                sf::RectangleShape button,
                sf::RectangleShape button2,
                sf::Event event,
                char *value,
                int change)
{
    int writevalue;
    sf::Vector2i p;

    if (event.type == sf::Event::MouseButtonPressed)
    {

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            p = sf::Mouse::getPosition(window);

            if (buttonPressed(p, button))
            {
                writevalue = atoi((const char *)value);
                if (writevalue < 99 && writevalue >= 0)
                {
                    flag = 1;
                    if (writevalue >= 95)
                    {
                        writevalue = writevalue++;
                        writer(file, writevalue);
                    }
                    else
                    {
                        writevalue = writevalue + change;
                        writer(file, writevalue);
                    }
                }
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            p = sf::Mouse::getPosition(window);

            if (buttonPressed(p, button2))
            {
                writevalue = atoi((const char *)value);
                if (writevalue < 99 && writevalue > 0)
                {
                    if(flag)
                    {
                        writevalue = writevalue - change;
                        writer(file, writevalue);
                    }
                }
            }
        }
    }
}

int main()
{
    time_t my_t, m_t;
    // Create the main window
    sf::Texture t;
    sf::RectangleShape decreasebutton;
    sf::RectangleShape increasebutton;
    sf::RectangleShape volumeUp;
    sf::RectangleShape volumeDown;
    button decrease;
    char conversation;
    // Create a graphical text to display
    sf::Font font;
    int i = 0;
    unsigned int time;
    if (!font.loadFromFile("ArialCE.ttf"))
    {
        return EXIT_FAILURE;
    }

    // Load a music to play
    while (window.isOpen())
    {
        my_t = clock();
        // Process events
        sf::Event event;

        while (window.pollEvent(event))
        {
            // Close windo  w: exit
            if (event.type == sf::Event::Closed)
                window.close();
            brightness("/sys/class/backlight/amdgpu_bl1/brightness", increasebutton, decreasebutton, event, v.brightness, 5);
            brightness("volume", volumeDown, volumeUp, event, v.volume, 1);
        }

        // Clear screen
        window.clear();

        // Draw the string
        time = my_t / CLOCKS_PER_SEC;

        if (((time % 300) == 0))
        {
            if(flag)
                reader("/sys/class/power_supply/BAT0/capacity", v.charge);
        }
        if(flag)
        {
            reader("/sys/class/backlight/amdgpu_bl1/brightness", v.brightness);
            reader("volume", v.volume);
        }

        // text object
        sf::Text brtext(v.brightness, font, 50);
        sf::Text text(v.charge, font, 50);
        sf::Text vtext(v.volume, font, 50);

        if (atoi(v.charge) <= 10 || atof(v.brightness) <= 10 || atoi(v.volume) <= 10)
        {
            text.setFillColor(sf::Color::Red);
            flag = 0;
        }
        else
        {
            text.setFillColor(sf::Color::White);
            flag = 1;
        }
        
        text.setPosition(sf::Vector2f(0, 25));
        brtext.setPosition(sf::Vector2f(0, 165));
        vtext.setPosition(sf::Vector2f(0, 340));

        decreasebutton.setPosition(sf::Vector2f(0, 240));
        increasebutton.setPosition(sf::Vector2f(50, 240));

        volumeUp.setPosition(sf::Vector2f(0, 400));
        volumeDown.setPosition(sf::Vector2f(50, 400));

        increasebutton.setSize(sf::Vector2f(40, 40));
        decreasebutton.setSize(sf::Vector2f(40, 40));

        volumeUp.setSize(sf::Vector2f(40, 40));
        volumeDown.setSize(sf::Vector2f(40, 40));

        increasebutton.setFillColor(sf::Color::White);
        decreasebutton.setFillColor(sf::Color::White);

        volumeUp.setFillColor(sf::Color::White);
        volumeDown.setFillColor(sf::Color::White);
        if (atoi(v.charge) >= 95)
        {
            text.setFillColor(sf::Color::Green);
        }
        else
        {
            text.setFillColor(sf::Color::White);
        }
        window.draw(text);
        window.draw(brtext);
        window.draw(vtext);

        window.draw(decreasebutton);
        window.draw(increasebutton);
        window.draw(volumeUp);
        window.draw(volumeDown);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}