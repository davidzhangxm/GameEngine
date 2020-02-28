//
//  RenderEngine.hpp
//  GameEngine
//
//  Created by Xinming Zhang on 2/27/20.
//  Copyright © 2020 Xinming Zhang. All rights reserved.
//

#ifndef RenderEngine_hpp
#define RenderEngine_hpp

class RenderEngine
{
public:
    RenderEngine();
    ~RenderEngine() = default;
    
    void init();
    void render(float elapsedTime);
    void update();
private:
    
};

#endif /* RenderEngine_hpp */
