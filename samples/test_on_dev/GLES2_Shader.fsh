//
//  Shader.fsh
//  glgame
//
//  Created by gil on 10/27/12.
//  Copyright (c) 2012 sky923. All rights reserved.
//

varying vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}

