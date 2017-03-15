//
//  vorReg.hpp
//  VoronoiArt
//
//  Created by Cinda Heeren on 3/14/16.
//  Copyright © 2016 Cinda Heeren. All rights reserved.
//

#ifndef vorReg_hpp
#define vorReg_hpp

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
using namespace std;

// Here is a small helper for you ! Have a look.
//#include "ResourcePath.hpp"

class vorReg:public sf::Drawable, public sf::Transformable {
    
public:
    
    vorReg(sf::Vector2u c, const vector<sf::Vector2u>& ctrs, sf::Vector2u tSize);
    
    //updates the array of pixels that represents the voronoi image
    void updatePixels(sf::Uint8 *&pixels, sf::Vector2u dimensions, const sf::Image &image) const;
    
private:
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();
        
        // draw the vertex array
        target.draw(verts, states);
    }
    
    sf::VertexArray verts;
    
    sf::Vector2u closeCenter(sf::Vector2u p, const vector<sf::Vector2u> & cs);

};



#endif /* vorReg_hpp */
