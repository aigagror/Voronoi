//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//

#include <SFML/Graphics.hpp>

#include "ResourcePath.hpp"
#include "vorReg.hpp"
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <math.h>
/*
 To change between Voronoi and Pointillistic mode, change the value of POINTILLISM to 0 and 1 respectively.
 I'd also recommend changing NUM_CNTRS to a lower value if you want to go into Voronoi mode.
 */
#define NUMBER_OF_VP_PER_FRAME 100
#define NUM_CNTRS 100000
#define POINTILLISM 1
using namespace std;

//void captureBackground(sf::RectangleShape &background, sf::RenderWindow &window, sf::Texture &textureOfBackground) {
//    textureOfBackground.loadFromImage(window.capture());
//}

int main(int, char const**)
{
    
    // Load a texture to display
    sf::Texture texture;
    sf::Image image;
    if(!image.loadFromFile("/Users/Eddie/Downloads/cs225honors-master-e38f271bd13f27ff1d27a14ece5f964e218fbf34/cs225honors-Seurat-1abfb1cf039c6221ce9073ac47db5fcb20abdb92/VoronoiArt/parkPhoto.jpg")) {
        return EXIT_FAILURE;
    }
    texture.loadFromImage(image);
    
    cout << "Size of texture: " << texture.getSize().x << " x " << texture.getSize().y << endl;
    
    sf::Vector2u texSize(texture.getSize());
    int scale = 1;
    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(scale*texSize.x, scale*texSize.y), "Voronoi Seurat");
    window.setVerticalSyncEnabled(true);
    cout << "Size of window: " << window.getSize().x << " x " << window.getSize().y << endl;
    
    vector<sf::Vector2u> centers;
    srand (time(NULL));
    // create numCenters random points
    for (int i = 0; i < NUM_CNTRS; i++) {
        int x = rand() % texSize.x;
        int y = rand() % texSize.y;
        centers.push_back(sf::Vector2u(x,y));
    }
    
    vector< vorReg > vorPts; // voronoiPoints
    vector<sf::CircleShape> dots; //pointillistic dots
    if (POINTILLISM) {
        float radius = 2 * sqrtf(texSize.x * texSize.y / M_PI / NUM_CNTRS);
        for (int i = 0; i < NUM_CNTRS; i++) {
            sf::Color color = image.getPixel(centers[i].x, centers[i].y);
            color.a = 128;
            sf::CircleShape circle(radius, 10);
            circle.setPosition(centers[i].x - radius, centers[i].y - radius);
            circle.scale(1 + min(rand() % 400, min(rand() % 400, rand() % 400)) / 100.0, 1);
            float rotation = (rand() % 90) * pow(color.r / float(color.b == 0 ? 1 : color.b), 10);
            
            if (rand() % 100 < 40) {//change the color one of the dots every so often
                for (int i = 0; i < 3; i++) {
                    bool brightenOrDarken = rand() % 2;
                    float variation = rand() % 20 / 100.0;
                    variation *= brightenOrDarken ? 1 : -1;
                    switch (i) {
                        case 0:
                            if (int(color.r) * (1 + variation) > 255) {
                                color.r = 255;
                            }
                            else {
                                color.r *= (1 + variation);
                            }
                            break;
                        case 1:
                            if (int(color.g) * (1 + variation) > 255) {
                                color.g = 255;
                            }
                            else {
                                color.g *= (1 + variation);
                            }
                            break;
                        case 2:
                            if (int(color.b) * (1 + variation) > 255) {
                                color.b = 255;
                            }
                            else {
                                color.b *= (1 + variation);
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            circle.setFillColor(color);

            bool leftOrRight = rand() % 2;
            rotation *= leftOrRight ? 1 : -1;
            circle.rotate(rotation);
            dots.push_back(circle);
        }
    }
    else {
        cout << "Processing voronoiPoints" << endl;
        sf::Clock voronoiTime;
        
        for (int i = 0; i < NUM_CNTRS; i++){
            vorReg v(centers[i],centers,texSize);
            v.scale(scale,scale);
            switch (i) {
                case NUM_CNTRS / 10:
                    cout << "10%" << endl;
                    break;
                case NUM_CNTRS / 10 * 2:
                    cout << "20%" << endl;
                    break;
                case NUM_CNTRS / 10 * 3:
                    cout << "30%" << endl;
                    break;
                case NUM_CNTRS / 10 * 4:
                    cout << "40%" << endl;
                    break;
                case NUM_CNTRS / 10 * 5:
                    cout << "50%" << endl;
                    break;
                case NUM_CNTRS / 10 * 6:
                    cout << "60%" << endl;
                    break;
                case NUM_CNTRS / 10 * 7:
                    cout << "70%" << endl;
                    break;
                case NUM_CNTRS / 10 * 8:
                    cout << "80%" << endl;
                    break;
                case NUM_CNTRS / 10 * 9:
                    cout << "90%" << endl;
                    break;
                default:
                    break;
            }
            //v.move(scale*texSize.x,0);
            vorPts.push_back(v);
        }
        cout << "Finished calculating the voronoiPoints" << endl;
        cout << "Took " << voronoiTime.getElapsedTime().asSeconds() << "seconds" << endl;
    }
    
    sf::RectangleShape canvas(sf::Vector2f(texSize.x, texSize.y));
    canvas.setTexture(&texture);
    canvas.scale(scale,scale);
    canvas.setPosition(0,0);
    
    //Setup pixel array and set up tempTex and tempSprite with it
    int size = window.getSize().x * window.getSize().y * 4;
    sf::Uint8 *pixels = new sf::Uint8[size];
    for (int i = 0; i < size; i++) {
        pixels[i] = 125;
    }
    sf::Texture tempTex;
    tempTex.create(window.getSize().x, window.getSize().y);
    tempTex.update(pixels);
    sf::Sprite tempSprite;
    tempSprite.setTexture(tempTex);
    tempSprite.setPosition(0, 0);
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close(); }
        }
        //int startIndex = 0;
        //texture.loadFromFile(resourcePath() + "parkPhoto.jpg");
        
        if (POINTILLISM) {
            window.clear(sf::Color(180,180,180));
            for (int i = 0; i < NUM_CNTRS; i++) {
                window.draw(dots[i]);
            }
            window.display();
            sf::sleep(sf::seconds(1));
        }
        else {
            //Show original photo
            window.clear();
            window.draw(canvas);
            window.display();
            cout << "Original Photo:" << endl;
            sf::sleep(sf::seconds(1));
            
            //Show a blank space
            window.clear();
            for (int i = 0; i < size; i++) {
                pixels[i] = image.getPixelsPtr()[i];
            }
            tempTex.update(pixels);
            window.draw(tempSprite);
            window.display();
            cout << "Updated pixels" << endl;
            sf::sleep(sf::seconds(1));
            
            //Start the animation
            for (int i = NUMBER_OF_VP_PER_FRAME - 1; i < NUM_CNTRS; i += NUMBER_OF_VP_PER_FRAME) {
                window.clear();
                for (int j = 0; j < NUMBER_OF_VP_PER_FRAME; j++) {
                    vorPts[i - j].updatePixels(pixels, sf::Vector2u(window.getSize()), image);
                }
                tempTex.update(pixels);
                window.draw(tempSprite);
                window.display();
                cout << "i: " << i << endl;
                
                //            for (int j = startIndex; j < i; j++) {
                //                window.draw(vorPts[j], &texture);
                //            }
                //            if ((i - startIndex) > 500) {
                //                startIndex = i + 1;
                //                texture.update(pixels);
                //            }
            }
            cout << "Finished drawing" << endl;
            sf::sleep(sf::seconds(3));
        }
        
        
    }
    
    delete[] pixels;
    
    return EXIT_SUCCESS;
}
