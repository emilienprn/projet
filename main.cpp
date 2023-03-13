/**
 * SPDX-License-Identifier: MPL-2.0+
 * @file main.cpp
 * @brief Tree extraction and visual exploration, requiring Imagine library.
 * @author Pascal Monasse <monasse@imagine.enpc.fr>
 *
 * Copyright (c) 2018 Pascal Monasse
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Mozilla Public License as
 * published by the Mozilla Foundation, either version 2.0 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * Mozilla Public License for more details.
 *
 * You should have received a copy of the Mozilla Public License
 * along with this program. If not, see <https://www.mozilla.org/en-US/MPL/2.0/>
 */

#include <Imagine/Images.h>
#include "../tree.h"
#include <iostream>
using namespace Imagine;


float Hc(Image<byte> im, std::vector<LsPoint> courbe, float mu){
    std::vector<float> Du;
    float min_du = 0;
    for(int i=0; i<courbe.size(); i++){
        FVector<float, 2> grad;
        if(courbe[i].x < im.width() and courbe[i].y < im.height()){
            grad = gradient(im, {courbe[i].x, courbe[i].y});
        }else{
            grad = {0.0f, 0.0f};
        }
        float Dux_norm = sqrt(pow(grad.x(), 2)+pow(grad.y(), 2));
        if(i==0 or Dux_norm < min_du){
            min_du = Dux_norm;
        }
        Du.push_back(Dux_norm);
    }

    //on compte

    size_t numerateur   = 0;
    size_t denominateur = 0;

    for(int i=0; i<courbe.size(); i++){
        if(Du[i] > mu){
            numerateur++;
        }
        if(Du[i] >  min_du){
            denominateur++;
        }
    }
    return float(numerateur)/float(denominateur);
}

int main(int argc, char* argv[]) {
    if(argc!=2) {
        std::cerr << "Usage: " << argv[0] << " imageFile" << std::endl;
        return 1;
    }
    Image<byte> im;
    if(! load(im, argv[1])) {
        std::cerr << "Error loading image " << argv[1] << std::endl;
        return 1;
    }
    openWindow(im.width()+1, im.height()+1);
    display(im);

    LsTree tree(im.data(), im.width(), im.height());
    std::cout << "Shapes: " << tree.iNbShapes << std::endl;
    std::cout << "Hc()= " << Hc(im, tree.shapes[0].contour, 100) << std::endl;

    endGraphics();
    return 0;
}


