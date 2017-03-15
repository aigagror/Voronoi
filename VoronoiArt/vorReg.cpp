//
//  vorReg.cpp
//  VoronoiArt
//
//  Created by Cinda Heeren on 3/14/16.
//  Copyright © 2016 Cinda Heeren. All rights reserved.
//

#include "vorReg.hpp"
#include <iostream>

sf::Vector2u vorReg::closeCenter(sf::Vector2u p, const vector<sf::Vector2u> & cs){
    sf::Vector2u min = cs[0];
    float_t minDist = (p.x - min.x)*(p.x - min.x) + (p.y - min.y)*(p.y - min.y);
    
    for (int i = 0; i < cs.size();i++){
        float_t d = (p.x - cs[i].x)*(p.x - cs[i].x)
        + (p.y - cs[i].y)*(p.y - cs[i].y);
        if (d < minDist){
            min = cs[i];
            minDist = d;
        }
    }
    return min;

}

void vorReg::updatePixels(sf::Uint8 *&pixels, sf::Vector2u dimensions, const sf::Image &image) const {
    unsigned long int numPixels = verts.getVertexCount();
    int startIndexOfColorVert = 4 * (verts[0].texCoords.y * dimensions.x + verts[0].texCoords.x);//get subset of indices that represent the fill color
    for (int i = 0; i < numPixels; i++) {
        int startIndexVert = 4 * (verts[i].position.y * dimensions.x + verts[i].position.x);
        pixels[startIndexVert] = image.getPixelsPtr()[startIndexOfColorVert];
        pixels[startIndexVert + 1] = image.getPixelsPtr()[startIndexOfColorVert + 1];
        pixels[startIndexVert + 2] = image.getPixelsPtr()[startIndexOfColorVert + 2];
        pixels[startIndexVert + 3] = image.getPixelsPtr()[startIndexOfColorVert + 3];
    }
}


vorReg::vorReg(sf::Vector2u c, const vector<sf::Vector2u>& ctrs, sf::Vector2u tSize){
    
    verts.setPrimitiveType(sf::Points);
    
    vector< bool > done(tSize.x*tSize.y);
    for (int i = 0; i< tSize.x*tSize.y; i++) done[i] = false;
    
    queue<sf::Vector2u> q;
    q.push(c);
    done[c.y * tSize.x + c.x]=true;
    while (!q.empty()){
        sf::Vector2u p = q.front();
        q.pop();
        sf::Vertex pv((sf::Vector2f(p)));
        pv.texCoords = sf::Vector2f(c);
        verts.append(pv);
        
        //left neighbor
        if (int(p.x) -1 >= 0) {
            sf::Vector2u pL(p.x-1, p.y);
            if (! done[pL.y * tSize.x + pL.x])
                if (closeCenter(pL,ctrs) == c) {
                    q.push(pL);
                    done[pL.y * tSize.x + pL.x] = true;
                }
        }
        //up neighbor
        if (int(p.y-1) >= 0) {
            sf::Vector2u pU(p.x, p.y-1);
            if (! done[pU.y * tSize.x + pU.x])
                if (closeCenter(pU,ctrs) == c) {
                    q.push(pU);
                    done[pU.y * tSize.x + pU.x] = true;
                }
            
        }
        //right neighbor
        if (p.x+1 < tSize.x) {
            sf::Vector2u pR(p.x+1, p.y);
            if (! done[pR.y * tSize.x + pR.x])
                if (closeCenter(pR,ctrs) == c){
                    q.push(pR);
                    done[pR.y * tSize.x + pR.x] = true;
                }
            
        }
        // down neighbor
        if (p.y+1 < tSize.y) {
            sf::Vector2u pD(p.x, p.y+1);
            if (! done[pD.y * tSize.x + pD.x])
                if (closeCenter(pD,ctrs) == c){
                    q.push(pD);
                    done[pD.y * tSize.x + pD.x] = true;
                }
        }
    }
}

