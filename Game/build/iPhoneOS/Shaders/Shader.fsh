//
//  Shader.fsh
//  ShootTest
//
//  Created by Amine Rehioui on 10-08-04.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
